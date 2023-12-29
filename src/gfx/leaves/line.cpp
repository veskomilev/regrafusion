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

void Line::draw(std::shared_ptr<QPainter> painter)
{;
    Leaf::draw(painter);

    painter->setPen(color_);
    painter->drawLine(line_);

    unapplyLocalTransformations(painter);
}
