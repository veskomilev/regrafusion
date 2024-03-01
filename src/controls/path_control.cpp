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

    painter->setBrush(Qt::black);
    painter->setPen(Qt::transparent);

    // in order for selection points not to be weirdly stretched and squished by leaf scalings, do the following:
    // 1. turn off world transformations (WT)
    // 2. map selection point coordinates to WT space
    // 3. draw them with transformations still disabled
    // 4. turn WT back on
    painter->setWorldMatrixEnabled(false);

    for (QPointF& point : points) {
        QPointF mapped = mapPointToLeafInBranch(ctx_p, leaf_p, point, depth);
        if (getPointDistance(mouse_position_, mapped) > kPopUpDistance)
            continue;

        painter->drawEllipse(QRectF(mapped.rx() - kRadius, mapped.ry() - kRadius, kRadius * 2, kRadius * 2));
    }

    painter->setWorldMatrixEnabled(true);
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
