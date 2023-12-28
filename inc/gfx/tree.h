// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef TREE_H
#define TREE_H

#include <vector>
#include <QtGlobal>

#include "branch.h"

class Tree
{
public:
    Tree();

private:
    // disable copy and assignment ctors
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    std::vector<std::unique_ptr<Branch>> branches_;
    uint num_branches_to_draw_;
};

#endif // TREE_H
