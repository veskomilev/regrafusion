// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

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

    std::shared_ptr<LeafIdentifier> leaf_id = ctx_p->leafIdentifier();
    // each and every branch has at least one spawn (branching) point
    leaves_.push_back(std::make_shared<SpawnPoint>(ctx_p));
    leaves_[0]->setTransformationMatrix(leaves_[0]->matrix().translate(60, 0).rotate(-10).scale(0.98, 0.98));
    leaf_id->registerLeaf(leaves_[0]);

    // some hardcoded leaves for now:
    leaves_.push_back(std::make_shared<Circle>(ctx_p, 15, Qt::green));
    leaves_[1]->setTransformationMatrix(leaves_[1]->matrix().translate(-10, -10));
    leaf_id->registerLeaf(leaves_[1]);

    leaves_.push_back(std::make_shared<Circle>(ctx_p, 20, Qt::red));
    leaves_[2]->setTransformationMatrix(leaves_[2]->matrix().scale(1, 0.5).translate(0, 50));
    leaf_id->registerLeaf(leaves_[2]);

    leaves_.push_back(std::make_shared<Circle>(ctx_p, 30, Qt::blue));
    leaves_[3]->setTransformationMatrix(leaves_[3]->matrix().rotate(30).scale(1, 0.2).translate(60, 0));
    leaf_id->registerLeaf(leaves_[3]);

    leaves_.push_back(std::make_shared<Line>(ctx_p, QLineF(0, 0, 100, 100), QColor(200, 0, 200)));
    leaves_[4]->setTransformationMatrix(leaves_[4]->matrix().translate(0, -30));
    leaf_id->registerLeaf(leaves_[4]);

    leaves_.push_back(std::make_shared<Rectangle>(ctx_p, QRectF(20, -40, 40, 20), QColor(0, 0, 200)));
    leaf_id->registerLeaf(leaves_[5]);

    std::shared_ptr<Path> path = std::make_shared<Path>(ctx_p, QColor(0, 150, 0));
    path->addPoint(QPointF(-40, 30));
    path->addPoint(QPointF(40, 30));
    path->addPoint(QPointF(32, 12));
    path->addPoint(QPointF(14, -20));
    path->addPoint(QPointF(-20, 20));
    path->addPoint(QPointF(-20, -30));
    leaves_.push_back(path);
    leaves_[6]->setTransformationMatrix(leaves_[6]->matrix().translate(0, 70));
    leaf_id->registerLeaf(leaves_[6]);
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
