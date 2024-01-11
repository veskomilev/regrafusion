// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QPainterPath>

#include "gfx/leaves/path.h"
#include "rgf_ctx.h"

Path::Path(std::weak_ptr<RgfCtx> ctx, QColor color) :
    Leaf {ctx, leaf_type_t::path},
    color_(color)
{

}

Path::~Path()
{

}

void Path::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    if (ctx_p == nullptr)
        return;


    Leaf::draw(painter, color_id_painter);

    QPainterPath path;

    for (QPointF& point : points_) {
        path.lineTo(point);
    }

    path.closeSubpath();

    if (selected_ && ctx_p->getMode() == RgfCtx::mode_t::edit) {
        painter->setPen(QColor(0, 0, 0, 255));
    } else {
        painter->setPen(QColor(0, 0, 0, 0));
    }
    painter->setBrush(color_);
    painter->drawPath(path);

    if (ctx_p->getMode() == RgfCtx::mode_t::edit) {
        color_id_painter->setPen(QColor(0, 0, 0, 0));
        color_id_painter->setBrush(color_id_);
        color_id_painter->drawPath(path);
    }

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}

void Path::addPoint(QPointF point)
{
    points_.push_back(point);
}
