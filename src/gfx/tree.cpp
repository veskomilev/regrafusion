// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/tree.h"

Tree::Tree(uint num_branches_to_draw) :
    num_branches_to_draw_(num_branches_to_draw)
{
    branches_.push_back(std::make_unique<Branch>());
}

void Tree::draw(std::shared_ptr<QPainter> painter)
{
    for (auto &branch : branches_) {
        branch->draw(painter, num_branches_to_draw_);
    }
}
