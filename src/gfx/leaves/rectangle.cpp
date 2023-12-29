// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QPainterPath>

#include "gfx/leaves/rectangle.h"

Rectangle::Rectangle(QRectF rectangle, QColor color) :
    Leaf {leaf_type_t::rectangle},
    rectangle_(rectangle),
    color_(color)
{

}

Rectangle::~Rectangle()
{

}

void Rectangle::draw(std::shared_ptr<QPainter> painter)
{;
    Leaf::draw(painter);

    painter->setPen(QColor(0, 0, 0, 0));
    painter->setBrush(color_);
    painter->drawRect(rectangle_);

    unapplyLocalTransformations(painter);
}
