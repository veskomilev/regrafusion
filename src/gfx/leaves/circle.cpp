// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QPainterPath>

#include "gfx/leaves/circle.h"

Circle::Circle(qreal radius, QColor color) :
    Leaf {leaf_type_t::circle},
    radius_(abs(radius)),
    color_(color)
{

}

Circle::~Circle()
{

}

void Circle::draw(std::shared_ptr<QPainter> painter)
{;
    Leaf::draw(painter);

    QPainterPath circle;
    circle.addEllipse(QRectF(-radius_, -radius_, radius_ * 2, radius_ * 2));

    painter->setPen(QColor(0, 0, 0, 0));
    painter->setBrush(color_);

    // TODO: don't draw a circle (or any graphics for that matter) if it's going to be less than 1 pixel wide in any dimension
    painter->drawPath(circle);

    unapplyLocalTransformations(painter);
}
