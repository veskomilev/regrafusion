// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <chrono>

#include "gfx/branch.h"
#include "gfx/leaves/circle.h"
#include "gfx/leaves/spawnpoint.h"

Branch::Branch()
{
    // each and every branch has at least one spawn (branching) point
    leaves_.push_back(std::make_unique<SpawnPoint>());
    leaves_[0]->setTransformationMatrix(leaves_[0]->matrix().translate(60, 0).rotate(-10).scale(0.98, 0.98));

    // some hardcoded leaves for now:
    leaves_.push_back(std::make_unique<Circle>(15, Qt::green));
    leaves_[1]->setTransformationMatrix(leaves_[1]->matrix().translate(-10, -10));

    leaves_.push_back(std::make_unique<Circle>(20, Qt::red));
    leaves_[2]->setTransformationMatrix(leaves_[2]->matrix().scale(1, 0.5).translate(0, 50));

    leaves_.push_back(std::make_unique<Circle>(30, Qt::blue));
    leaves_[3]->setTransformationMatrix(leaves_[3]->matrix().rotate(30).scale(1, 0.2).translate(60, 0));
}

void Branch::draw(std::shared_ptr<QPainter> painter, uint num_iterations, BranchStatistics& stats)
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
            leaf->draw(painter);

        } else if (num_iterations > 0) {
            leaf->draw(painter);

            // exclude drawing time of subbranches from this branch's stats
            branching_start = std::chrono::steady_clock::now();
            draw(painter, num_iterations, stats);
            branching_end = std::chrono::steady_clock::now();

            leaf->unapplyLocalTransformations(painter);
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
