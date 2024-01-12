// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QPainterPath>

#include "gfx/leaves/rectangle.h"
#include "rgf_ctx.h"

Rectangle::Rectangle(std::weak_ptr<RgfCtx> ctx, QRectF rectangle, QColor color) :
    Leaf {ctx, leaf_type_t::rectangle},
    rectangle_(rectangle),
    color_(color)
{

}

Rectangle::~Rectangle()
{

}

void Rectangle::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    if (ctx_p == nullptr)
        return;


    Leaf::draw(painter, color_id_painter, depth);

    if (selected_ && ctx_p->getMode() == RgfCtx::mode_t::edit) {
        painter->setPen(QColor(0, 0, 0, 255));
    } else {
        painter->setPen(QColor(0, 0, 0, 0));
    }
    painter->setBrush(color_);
    painter->drawRect(rectangle_);

    if (ctx_p->getMode() == RgfCtx::mode_t::edit) {
        color_id_painter->setPen(QColor(0, 0, 0, 0));
        color_id_painter->setBrush(getUniqueColor(depth));
        color_id_painter->drawRect(rectangle_);
    }

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}
