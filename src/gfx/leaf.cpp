// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/leaf.h"

Leaf::Leaf(leaf_type_t type) :
    type_(type),
    matrix_(QTransform())
{
}

Leaf::~Leaf()
{
}

void Leaf::draw(std::shared_ptr<QPainter> painter)
{
    if (!matrix_.isIdentity()) {
        // combine param is true, so combine matrix_ with the existing transformations instead of overwriting them
        painter->setWorldTransform(matrix_, true);
    }

    // draw matrix here...
}

bool Leaf::setTransformationMatrix(QTransform matrix)
{
    if (!matrix.isInvertible())
        return false;

    matrix_ = matrix;
    return true;
}
