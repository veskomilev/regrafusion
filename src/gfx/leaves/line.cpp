// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QPainterPath>

#include "gfx/leaves/line.h"

Line::Line(QLineF line, QColor color) :
    Leaf {leaf_type_t::line},
    line_(line),
    color_(color)
{

}

Line::~Line()
{

}

void Line::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter)
{
    Leaf::draw(painter, color_id_painter);

    painter->setPen(color_);
    painter->drawLine(line_);

    color_id_painter->setPen(color_id_);
    color_id_painter->drawLine(line_);

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}
