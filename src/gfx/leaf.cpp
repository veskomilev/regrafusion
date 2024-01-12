// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/leaf.h"
#include "rgf_ctx.h"

Leaf::Leaf(std::weak_ptr<RgfCtx> ctx, leaf_type_t type) :
    ctx_(ctx),
    color_id_(QColor(255, 255, 255)),
    selected_(false),
    type_(type),
    matrix_(QTransform())
{
}

Leaf::~Leaf()
{
}

void Leaf::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    if (ctx_p == nullptr)
        return;

    if (!matrix_.isIdentity()) {
        // combine param is true, so combine matrix_ with the existing transformations instead of overwriting them
        painter->setWorldTransform(matrix_, true);
        color_id_painter->setWorldTransform(matrix_, true);
    }

    if (ctx_p->getMode() != RgfCtx::mode_t::edit || !selected_)
        return;

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
}

bool Leaf::setTransformationMatrix(QTransform matrix)
{
    if (!matrix.isInvertible())
        return false;

    matrix_ = matrix;
    return true;
}

QPointF Leaf::toLocalSpace(QPointF coordinate)
{
    return matrix_.inverted().map(coordinate);
}

QColor Leaf::getUniqueColor(uint depth)
{
    QColor result = color_id_;

    // sanity verification
    depth &= 0x3FF;

    result.setRgb(result.rgb() | depth);

    return result;
}
