// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <cstddef>
#include <QMouseEvent>
#include <QPainterPath>

#include "controls/path_control.h"
#include "gfx/leaf.h"
#include "gfx/leaves/path.h"
#include "math_utils.h"
#include "rgf_ctx.h"

PathControl::PathControl(std::weak_ptr<RgfCtx> ctx, std::weak_ptr<Leaf> leaf) :
    Control {ctx, leaf},
    mouse_position_(QPointF(0, 0))
{
    type_ = leaf_type_t::path;
}

PathControl::~PathControl()
{
}

void PathControl::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    auto leaf_p = leaf_.lock();
    if (leaf_p == nullptr)
        return;

    if (leaf_p->getType() != type_)
        return;

    std::vector<QPointF> points = std::dynamic_pointer_cast<Path>(leaf_p)->points();
    if (points.size() == 0)
        return;

    auto ctx_p = ctx_.lock();
    if (ctx_p == nullptr)
        return;

    if (depth != ctx_p->getSelectedLeafDepth())
        return;

    View view = ctx_p->getView();
    qreal radius = kRadius / view.scale;
    color_id_painter->setBrush(Qt::black);

    for (QPointF& point : points) {
        color_id_painter->drawEllipse(QRectF(point.rx() - radius, point.ry() - radius, radius * 2, radius * 2));
    }
}

bool PathControl::handleEvent(QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        mouse_position_ = mouseEvent->pos();

        auto ctx_p = ctx_.lock();
        if (ctx_p != nullptr) {
            ctx_p->refresh();
        }
    }

    return false;
}
