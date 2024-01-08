// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef TREE_H
#define TREE_H

#include <vector>
#include <QtGlobal>

#include "branch.h"
#include "leaf_identifier.h"

struct TreeStatistics
{
    std::vector<uint> render_time_us;
    std::vector<uint> first_branch_render_time_us;
    std::vector<uint> last_branch_render_time_us;
    std::vector<uint> avg_branch_render_time_us;
};

class Tree
{
public:
    Tree(std::shared_ptr<LeafIdentifier> leaf_identifier, uint num_branches_to_draw);

    TreeStatistics& draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter);

    void setNumBranches(uint num_branches) { num_branches_to_draw_ = num_branches; }

    uint getNumBranches() { return num_branches_to_draw_; }

private:
    // disable copy and assignment ctors
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    std::shared_ptr<LeafIdentifier> leaf_identifier_;

    std::vector<std::unique_ptr<Branch>> branches_;

    uint num_branches_to_draw_;

    TreeStatistics stats_;

    static constexpr uint kMaxStatsSampleSize = 100;
};

#endif // TREE_H
