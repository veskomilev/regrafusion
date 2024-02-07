// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <chrono>

#include "common.h"
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

QTransform Tree::getSpawnPointTransformation()
{
    if (branches_.size() == 0) {
        return QTransform();
    }

    // TODO: do something here if there are ever more than one branches
    return branches_[0]->getSpawnPointTransformation();
}

void Tree::deleteLeaf(std::shared_ptr<Leaf> leaf)
{
    for (auto &branch : branches_) {
        branch->deleteLeaf(leaf);
    }
}

void Tree::addShape(leaf_type_t shape_type, QPointF position, qreal scale)
{
    if (branches_.size() == 0) {
        return;
    }

    // TODO: do something here if there are ever more than one branches
    return branches_[0]->addShape(shape_type, position, scale);
}
