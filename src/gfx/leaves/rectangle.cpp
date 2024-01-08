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

void Rectangle::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter)
{
    Leaf::draw(painter, color_id_painter);

    painter->setPen(QColor(0, 0, 0, 0));
    painter->setBrush(color_);
    painter->drawRect(rectangle_);

    color_id_painter->setPen(QColor(0, 0, 0, 0));
    color_id_painter->setBrush(color_id_);
    color_id_painter->drawRect(rectangle_);

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}
