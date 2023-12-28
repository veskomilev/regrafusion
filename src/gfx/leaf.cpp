// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/leaf.h"

Leaf::Leaf(leaf_type_t type, QTransform matrix):
    type_(type),
    matrix_(matrix)
{

}
