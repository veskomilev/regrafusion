// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef BRANCH_H
#define BRANCH_H

#include <vector>

#include "leaf.h"

class Branch
{
public:
    Branch();

private:
    // disable copy and assignment ctors
    Branch(const Branch&) = delete;
    Branch& operator=(const Branch&) = delete;

    std::vector<std::unique_ptr<Leaf>> leaves_;
    std::unique_ptr<Leaf> spawn_point_;
};

#endif // BRANCH_H
