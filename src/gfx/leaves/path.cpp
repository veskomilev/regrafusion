// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QPainterPath>

#include "gfx/leaves/path.h"

Path::Path(QColor color) :
    Leaf {leaf_type_t::path},
    color_(color)
{

}

Path::~Path()
{

}

void Path::draw(std::shared_ptr<QPainter> painter)
{;
    Leaf::draw(painter);

    QPainterPath path;

    for (QPointF& point : points_) {
        path.lineTo(point);
    }

    path.closeSubpath();

    painter->setPen(QColor(0, 0, 0, 0));
    painter->setBrush(color_);
    painter->drawPath(path);

    unapplyLocalTransformations(painter);
}

void Path::addPoint(QPointF point)
{
    points_.push_back(point);
}
