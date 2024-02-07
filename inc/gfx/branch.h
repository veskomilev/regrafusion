// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef BRANCH_H
#define BRANCH_H

#include <vector>

#include "leaf.h"

class RgfCtx;

struct BranchStatistics
{
    uint first_branch_render_time_us;
    uint last_branch_render_time_us;
    std::vector<uint> branch_render_times_us;
    uint num_branches;
};

class Branch
{
public:
    Branch(std::weak_ptr<RgfCtx> ctx);

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint num_iterations, BranchStatistics& stats, uint depth);

    void deselect();

    QTransform getSpawnPointTransformation();

    void deleteLeaf(std::shared_ptr<Leaf> leaf);

    void addShape(leaf_type_t shape_type, QPointF position, qreal scale);

private:
    // disable copy and assignment ctors
    Branch(const Branch&) = delete;
    Branch& operator=(const Branch&) = delete;

    std::weak_ptr<RgfCtx> ctx_;

    std::vector<std::shared_ptr<Leaf>> leaves_;
    std::unique_ptr<Leaf> spawn_point_;
};

#endif // BRANCH_H
