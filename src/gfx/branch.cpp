// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <chrono>

#include "gfx/branch.h"
#include "gfx/leaves/circle.h"
#include "gfx/leaves/line.h"
#include "gfx/leaves/path.h"
#include "gfx/leaves/rectangle.h"
#include "gfx/leaves/spawnpoint.h"
#include "rgf_ctx.h"

Branch::Branch(std::weak_ptr<RgfCtx> ctx) :
    ctx_(ctx)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    assert(ctx_p != nullptr && "Branch was created for a non existant context");

    auto leaf = Leaf::constructNew(ctx_p, leaf_type_t::spawn_point);
    leaf->setTransformationMatrix(leaf->matrix().translate(60, 0).rotate(-10).scale(0.98, 0.98));
    leaves_.push_back(leaf);
}

void Branch::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint num_iterations, BranchStatistics& stats, uint depth)
{
    // each invocation of draw() immediately uses up one iteration
    num_iterations--;

    // TODO: add a branch's own proper transformation matrix, so that even the first branch can have all of its elements transformed

    std::chrono::steady_clock::time_point drawing_start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point branching_start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point branching_end = std::chrono::steady_clock::now();

    // use recursion, because the user may want to draw subbranches above some leaves of the current branch, but below others
    // and there's provision to allow for multiple spawn points in the future
    for (auto &leaf : leaves_) {

        if (!leaf->isSpawnPoint()) {
            leaf->draw(painter, color_id_painter, depth);

            // draw controls on the first iteration, so that they are on top of everything
            if (depth == 0 && leaf->isSelected()) {
                leaf->drawControls(painter);
            }

        } else if (num_iterations > 0) {
            leaf->draw(painter, color_id_painter, depth);

            // exclude drawing time of subbranches from this branch's stats
            branching_start = std::chrono::steady_clock::now();
            draw(painter, color_id_painter, num_iterations, stats, depth + 1);
            branching_end = std::chrono::steady_clock::now();

            leaf->unapplyLocalTransformations(painter);
            leaf->unapplyLocalTransformations(color_id_painter);
        }

    }

    std::chrono::steady_clock::time_point drawing_end = std::chrono::steady_clock::now();

    uint render_time_us;
    render_time_us = std::chrono::duration_cast<std::chrono::microseconds>(drawing_end - drawing_start).count();
    render_time_us -= std::chrono::duration_cast<std::chrono::microseconds>(branching_end - branching_start).count();

    // account for the decrement at the start
    if (num_iterations == stats.num_branches - 1) {
        stats.first_branch_render_time_us = render_time_us;
    }

    // if there are no more iterations left, this is the last branch
    if (num_iterations == 0) {
        stats.last_branch_render_time_us = render_time_us;
    }

    stats.branch_render_times_us.push_back(render_time_us);
}

void Branch::deselect()
{
    for (auto &leaf : leaves_) {
        leaf->deselect();
    }
}

QTransform Branch::getSpawnPointTransformation()
{
    // TODO: if there are ever more than one spawn points, do something about it
    for (auto &leaf : leaves_) {
        if (leaf->isSpawnPoint()) {
            return leaf->matrix();
        }
    }

    return QTransform();
}

void Branch::deleteLeaf(std::shared_ptr<Leaf> leaf)
{
    if (leaf == nullptr)
        return;

    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();
    assert(ctx_p != nullptr && "Branch exists for a non existant context");
    ctx_p->leafIdentifier()->unregisterLeaf(leaf);

    leaves_.erase(
        std::remove_if(
            leaves_.begin(),
            leaves_.end(),
            [leaf](std::shared_ptr<Leaf> element) { return element == leaf; }
            )
        );
}

std::shared_ptr<Leaf> Branch::addShape(leaf_type_t shape_type, QPointF position, qreal scale)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();
    assert(ctx_p != nullptr && "Branch exists for a non existant context");

    auto leaf = Leaf::constructNew(ctx_p, shape_type);
    leaf->matrix().scale(1 / scale, 1 / scale);
    leaf->matrix().translate(position.rx(), position.ry());
    leaves_.push_back(leaf);

    return leaf;
}
