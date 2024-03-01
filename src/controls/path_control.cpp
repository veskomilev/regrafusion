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
    mouse_position_(QPointF(0, 0)),
    previous_mouse_position_(QPointF(0, 0)),
    vertex_dragged_(false),
    dragged_vertex_index_(0),
    add_vertex_mode_(false)
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

    std::vector<QPointF> points = std::dynamic_pointer_cast<Path>(leaf_p)->points();
    if (points.size() == 0)
        return;

    auto ctx_p = ctx_.lock();
    if (ctx_p == nullptr)
        return;

    if (depth != ctx_p->getSelectedLeafDepth())
        return;

    // in order for selection points not to be weirdly stretched and squished by leaf scalings, do the following:
    // 1. turn off world transformations (WT)
    // 2. map selection point coordinates to WT space
    // 3. draw them with transformations still disabled
    // 4. turn WT back on
    painter->setWorldMatrixEnabled(false);

    if (add_vertex_mode_) {
        drawAddVertexMode(painter, ctx_p, leaf_p, points, depth);
    } else {
        drawMoveVertexMode(painter, ctx_p, leaf_p, points, depth);
    }

    painter->setWorldMatrixEnabled(true);
}

bool PathControl::handleEvent(QEvent *event)
{
    switch(event->type()) {

    case QEvent::MouseButtonPress:
        return handleMouseButtonPress(static_cast<QMouseEvent *>(event));

    case QEvent::MouseMove:
        return handleMouseMove(static_cast<QMouseEvent *>(event));

    case QEvent::MouseButtonRelease:
        return handleMouseButtonRelease(static_cast<QMouseEvent *>(event));

    case QEvent::KeyPress:
        handleKeyPressed(static_cast<QKeyEvent *>(event));
        break;

    case QEvent::KeyRelease:
        handleKeyReleased(static_cast<QKeyEvent *>(event));
        break;

    }

    return false;
}

void PathControl::drawMoveVertexMode(std::shared_ptr<QPainter> painter, std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF> &points, uint depth)
{
    painter->setBrush(Qt::black);
    painter->setPen(Qt::transparent);

    for (QPointF& point : points) {
        QPointF mapped = mapPointToLeafInBranch(ctx, leaf, point, depth);
        if (getPointDistance(mouse_position_, mapped) > kPopUpDistance)
            continue;

        painter->drawEllipse(QRectF(mapped.rx() - kRadius, mapped.ry() - kRadius, kRadius * 2, kRadius * 2));
    }
}

void PathControl::drawAddVertexMode(std::shared_ptr<QPainter> painter, std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF> &points, uint depth)
{
    side_t side = findClosestSideToCursor(ctx, leaf, points, depth);
    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::DashLine);
    painter->drawLine(mouse_position_, side.a);
    painter->drawLine(mouse_position_, side.b);
}

side_t PathControl::findClosestSideToCursor(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF> &points, uint depth)
{
    side_t side;

    size_t size = points.size();
    qreal min_cross = 0;

    for(size_t i = 0; i < size; i++) {

        QPointF point1 = points[i];
        QPointF point2;
        if (i == size - 1) {
            point2 = points[0];
        } else {
            point2 = points[i + 1];
        }

        QPointF mapped1 = mapPointToLeafInBranch(ctx, leaf, point1, depth);
        QPointF mapped2 = mapPointToLeafInBranch(ctx, leaf, point2, depth);

        qreal cross = crossProduct(mapped1 - mouse_position_, mapped2 - mouse_position_);
        if (cross < min_cross || i == 0) {
            min_cross = cross;
            side.a = mapped1;
            side.b = mapped2;
            side.ind = i;
        }
    }

    return side;
}

bool PathControl::handleMouseButtonPress(QMouseEvent *event)
{
    // GOTCHA: for some reason this correction is needed for this event, but not for handleMouseMove()
    mouse_position_ = event->pos() + View::kMouseClickCorrection;
    previous_mouse_position_ = mouse_position_;

    auto leaf_p = leaf_.lock();
    if (leaf_p == nullptr)
        return false;

    std::vector<QPointF>& points = std::dynamic_pointer_cast<Path>(leaf_p)->points();
    if (points.size() == 0)
        return false;

    auto ctx_p = ctx_.lock();
    if (ctx_p == nullptr)
        return false;

    if (add_vertex_mode_) {
        return addVertex(ctx_p, leaf_p, points);
    } else {
        return startDraggingVertex(ctx_p, leaf_p, points);
    }
}

bool PathControl::startDraggingVertex(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points)
{
    uint index = 0;
    for (QPointF& point : points) {
        QPointF mapped = mapPointToLeafInBranch(ctx, leaf, point, ctx->getSelectedLeafDepth());

        if (getPointDistance(mouse_position_, mapped) <= kPopUpDistance) {
            vertex_dragged_ = true;
            dragged_vertex_index_ = index;
            return true;
        }

        index++;
    }

    return false;
}

bool PathControl::addVertex(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points)
{
    uint depth = ctx->getSelectedLeafDepth();
    side_t side = findClosestSideToCursor(ctx, leaf, points, depth);
    points.insert(points.begin() + side.ind + 1, inverseMapPointToLeafInBranch(ctx, leaf, mouse_position_, depth));
    ctx->refresh();
    return true;
}

bool PathControl::handleMouseMove(QMouseEvent *event)
{
    bool event_blocked = false;
    mouse_position_ = event->pos();

    auto ctx_p = ctx_.lock();
    if (ctx_p == nullptr) {
        return event_blocked;
    }

    if (vertex_dragged_) {
        auto leaf_p = leaf_.lock();
        if (leaf_p != nullptr) {
            QPointF deltaPosition = leaf_p->fromSreenSpace(ctx_p, mouse_position_) - leaf_p->fromSreenSpace(ctx_p, previous_mouse_position_);
            std::vector<QPointF>& points = std::dynamic_pointer_cast<Path>(leaf_p)->points();
            points[dragged_vertex_index_].rx() += deltaPosition.x();
            points[dragged_vertex_index_].ry() += deltaPosition.y();
            event_blocked = true;
        }
    }

    previous_mouse_position_ = mouse_position_;
    ctx_p->refresh();

    return event_blocked;
}

bool PathControl::handleMouseButtonRelease(QMouseEvent *event)
{
    if (vertex_dragged_) {
        vertex_dragged_ = false;
        return true;
    }

    return false;
}

void PathControl::handleKeyPressed(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Control:
        add_vertex_mode_ = true;
        break;
    }
}

void PathControl::handleKeyReleased(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Control:
        add_vertex_mode_ = false;
        break;
    }
}
