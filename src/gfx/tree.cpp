// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <chrono>

#include "gfx/tree.h"

Tree::Tree(std::weak_ptr<RgfCtx> ctx, uint num_branches_to_draw) :
    ctx_(ctx),
    num_branches_to_draw_(num_branches_to_draw)
{
    branches_.push_back(std::make_unique<Branch>(ctx_));
}

TreeStatistics& Tree::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter)
{
    BranchStatistics branch_stats;
    branch_stats.num_branches = num_branches_to_draw_;

    std::chrono::steady_clock::time_point drawing_start = std::chrono::steady_clock::now();

    for (auto &branch : branches_) {
        branch->draw(painter, color_id_painter, num_branches_to_draw_, branch_stats, 0);
    }

    std::chrono::steady_clock::time_point drawing_end = std::chrono::steady_clock::now();

    stats_.render_time_us.push_back(std::chrono::duration_cast<std::chrono::microseconds>(drawing_end - drawing_start).count());
    stats_.first_branch_render_time_us.push_back(branch_stats.first_branch_render_time_us);
    stats_.last_branch_render_time_us.push_back(branch_stats.last_branch_render_time_us);
    stats_.avg_branch_render_time_us.push_back(vector_average<uint>(branch_stats.branch_render_times_us));

    if (stats_.render_time_us.size() > kMaxStatsSampleSize) {
        stats_.render_time_us.erase(stats_.render_time_us.begin());
    }

    if (stats_.first_branch_render_time_us.size() > kMaxStatsSampleSize) {
        stats_.first_branch_render_time_us.erase(stats_.first_branch_render_time_us.begin());
    }

    if (stats_.last_branch_render_time_us.size() > kMaxStatsSampleSize) {
        stats_.last_branch_render_time_us.erase(stats_.last_branch_render_time_us.begin());
    }

    if (stats_.avg_branch_render_time_us.size() > kMaxStatsSampleSize) {
        stats_.avg_branch_render_time_us.erase(stats_.avg_branch_render_time_us.begin());
    }

    return stats_;
}

void Tree::deselect()
{
    for (auto &branch : branches_) {
        branch->deselect();
    }
}
