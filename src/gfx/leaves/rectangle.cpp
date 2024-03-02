// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

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

std::shared_ptr<Rectangle> Rectangle::constructNew(std::weak_ptr<RgfCtx> ctx)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx.lock();
    assert(ctx_p != nullptr && "A non existant context was accessed");

    return std::make_shared<Rectangle>(ctx_p, kDefaultRectangle, Qt::red);
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

    drawControls(painter, color_id_painter, depth);

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}

void Rectangle::drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale)
{
    painter->setPen(QColor(0, 0, 0, 255));
    painter->setBrush(QColor(0, 0, 0, 0));

    QTransform t;
    t.scale(1 / scale, 1 / scale);
    t.translate(position.rx(), position.ry());
    painter->setWorldTransform(t, true);
    painter->drawRect(kDefaultRectangle);
    painter->setWorldTransform(t.inverted(), true);
}

void Rectangle::createControls()
{

}
