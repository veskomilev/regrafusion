// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/leaf.h"

Leaf::Leaf(leaf_type_t type) :
    type_(type),
    matrix_(QTransform()),
    color_id_(QColor(255, 255, 255)),
    selected_(false)
{
}

Leaf::~Leaf()
{
}

void Leaf::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter)
{
    if (!matrix_.isIdentity()) {
        // combine param is true, so combine matrix_ with the existing transformations instead of overwriting them
        painter->setWorldTransform(matrix_, true);
        color_id_painter->setWorldTransform(matrix_, true);
    }

    // in principle the side should be 1, but 1 pixel is too short of a length
    const uint side = 30;

    // draw the transformation matrix
    painter->setPen(QColor(128, 128, 128, 64));
    painter->drawLine(side, 0, side, side);
    painter->drawLine(0, side, side, side);

    painter->setPen(QColor(255, 0, 0, 64));
    painter->drawLine(0, side, 0, 0);

    painter->setPen(QColor(0, 0, 255, 64));
    painter->drawLine(side, 0, 0, 0);

    color_id_painter->setPen(color_id_);
    color_id_painter->drawLine(side, 0, side, side);
    color_id_painter->drawLine(0, side, side, side);
    color_id_painter->drawLine(0, side, 0, 0);
    color_id_painter->drawLine(side, 0, 0, 0);
}

bool Leaf::setTransformationMatrix(QTransform matrix)
{
    if (!matrix.isInvertible())
        return false;

    matrix_ = matrix;
    return true;
}
