// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QPainterPath>

#include "gfx/leaves/circle.h"
#include "rgf_ctx.h"

Circle::Circle(std::weak_ptr<RgfCtx> ctx, qreal radius, QColor color) :
    Leaf {ctx, leaf_type_t::circle},
    radius_(abs(radius)),
    color_(color)
{

}

Circle::~Circle()
{

}

std::shared_ptr<Circle> Circle::constructNew(std::weak_ptr<RgfCtx> ctx)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx.lock();
    assert(ctx_p != nullptr && "A non existant context was accessed");

    // just a wrapper to get to the private ctor
    struct CircleCtor : public Circle {
        CircleCtor(std::weak_ptr<RgfCtx> ctx, qreal radius, QColor color) :
            Circle {ctx, radius, color} {}
    };

    return std::make_shared<CircleCtor>(ctx_p, kDefaultRadius, Qt::red);
}

void Circle::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
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

    // TODO: don't draw a circle (or any graphics for that matter) if it's going to be less than 1 pixel wide in any dimension
    // TODO: don't draw a circle (or any graphics for that matter) if it's going to be outside the view area
    painter->drawEllipse(QRectF(-radius_, -radius_, radius_ * 2, radius_ * 2));

    if (ctx_p->getMode() == RgfCtx::mode_t::edit) {
        color_id_painter->setBrush(getUniqueColor(depth));
        color_id_painter->setPen(QColor(0, 0, 0, 0));
        color_id_painter->drawEllipse(QRectF(-radius_, -radius_, radius_ * 2, radius_ * 2));
    }

    drawControls(painter, color_id_painter, depth);

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}

void Circle::drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale)
{
    painter->setPen(QColor(0, 0, 0, 255));
    painter->setBrush(QColor(0, 0, 0, 0));

    QTransform t;
    t.scale(1 / scale, 1 / scale);
    t.translate(position.rx(), position.ry());
    painter->setWorldTransform(t, true);
    painter->drawEllipse(QRectF(-kDefaultRadius, -kDefaultRadius, kDefaultRadius * 2, kDefaultRadius * 2));
    painter->setWorldTransform(t.inverted(), true);
}

void Circle::createControls()
{

}
