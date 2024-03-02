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

PathControl::PathControl(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Path> leaf, uint leaf_depth) :
    Control {ctx, leaf, leaf_depth},
    mouse_position_(QPointF(0, 0)),
    previous_mouse_position_(QPointF(0, 0)),
    vertex_dragged_(false),
    dragged_vertex_index_(0),
    add_vertex_mode_(false),
    remove_vertex_mode_(false)
{
    type_ = leaf_type_t::path;
}

PathControl::~PathControl()
{
}

void PathControl::draw(std::shared_ptr<QPainter> painter)
{
    std::vector<QPointF> points = std::dynamic_pointer_cast<Path>(leaf_)->points();
    if (points.size() == 0)
        return;

    // in order for selection points not to be weirdly stretched and squished by leaf scalings, do the following:
    // 1. turn off world transformations (WT)
    // 2. map selection point coordinates to WT space
    // 3. draw them with transformations still disabled
    // 4. turn WT back on
    painter->setWorldMatrixEnabled(false);

    if (add_vertex_mode_) {
        drawAddVertexMode(painter, points);
    } else if (remove_vertex_mode_) {
        drawRemoveVertexMode(painter, points);
    } else {
        drawMoveVertexMode(painter, points);
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

void PathControl::drawMoveVertexMode(std::shared_ptr<QPainter> painter, std::vector<QPointF> &points)
{
    painter->setBrush(Qt::black);
    painter->setPen(Qt::transparent);

    for (QPointF& point : points) {
        QPointF mapped = mapLeafSpaceToScreenSpace(point);
        if (getPointDistance(mouse_position_, mapped) > kPopUpDistance)
            continue;

        painter->drawEllipse(QRectF(mapped.rx() - kRadius, mapped.ry() - kRadius, kRadius * 2, kRadius * 2));
    }
}

void PathControl::drawAddVertexMode(std::shared_ptr<QPainter> painter, std::vector<QPointF> &points)
{
    side_t side = findClosestSideToCursor(points);
    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::DashLine);
    painter->drawLine(mouse_position_, side.a);
    painter->drawLine(mouse_position_, side.b);
}

void PathControl::drawRemoveVertexMode(std::shared_ptr<QPainter> painter, std::vector<QPointF> &points)
{
    // do not allow triangles to degenerate to lines
    size_t size = points.size();
    if (size <= 3)
        return;

    int index_to_remove = -1;

    for(size_t i = 0; i < size; i++) {
        QPointF mapped = mapLeafSpaceToScreenSpace(points[i]);
        if (getPointDistance(mouse_position_, mapped) <= kPopUpDistance) {
            index_to_remove = i;
            break;
        }
    }

    // cursor is too far away from any vertex, don't do anything
    if (index_to_remove < 0)
        return;

    int index_a;
    if (index_to_remove == 0) {
        index_a = size - 1;
    } else {
        index_a = index_to_remove - 1;
    }

    int index_b;
    if (index_to_remove == size - 1) {
        index_b = 0;
    } else {
        index_b = index_to_remove + 1;
    }

    QPointF point_a = mapLeafSpaceToScreenSpace(points[index_a]);
    QPointF point_b = mapLeafSpaceToScreenSpace(points[index_b]);

    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::DashLine);
    painter->drawLine(point_a, point_b);
}

side_t PathControl::findClosestSideToCursor(std::vector<QPointF> &points)
{
    side_t side;

    size_t size = points.size();
    qreal min_weight = 0;

    for(size_t i = 0; i < size; i++) {

        QPointF point1 = points[i];
        QPointF point2;
        if (i == size - 1) {
            point2 = points[0];
        } else {
            point2 = points[i + 1];
        }

        QPointF mapped1 = mapLeafSpaceToScreenSpace(point1);
        QPointF mapped2 = mapLeafSpaceToScreenSpace(point2);

        qreal weight = getPointDistance(mapped1, mouse_position_) + getPointDistance(mapped2, mouse_position_);
        if (weight < min_weight || i == 0) {
            min_weight = weight;
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

    std::vector<QPointF>& points = std::dynamic_pointer_cast<Path>(leaf_)->points();
    if (points.size() == 0)
        return false;

    if (add_vertex_mode_) {
        return addVertex(points);
    } else if (remove_vertex_mode_) {
        return removeVertex(points);
    } else {
        return startDraggingVertex(points);
    }
}

bool PathControl::startDraggingVertex(std::vector<QPointF>& points)
{
    uint index = 0;
    for (QPointF& point : points) {
        QPointF mapped = mapLeafSpaceToScreenSpace(point);

        if (getPointDistance(mouse_position_, mapped) <= kPopUpDistance) {
            vertex_dragged_ = true;
            dragged_vertex_index_ = index;
            return true;
        }

        index++;
    }

    return false;
}

bool PathControl::addVertex(std::vector<QPointF>& points)
{
    side_t side = findClosestSideToCursor(points);
    points.insert(points.begin() + side.ind + 1, mapScreenSpaceToLeafSpace(mouse_position_));
    ctx_->refresh();
    return true;
}

bool PathControl::removeVertex(std::vector<QPointF> &points)
{
    // do not allow triangles to degenerate to lines
    size_t size = points.size();
    if (size <= 3)
        return true;

    int index_to_remove = -1;

    for(size_t i = 0; i < size; i++) {
        QPointF mapped = mapLeafSpaceToScreenSpace(points[i]);
        if (getPointDistance(mouse_position_, mapped) <= kPopUpDistance) {
            index_to_remove = i;
            break;
        }
    }

    // cursor is too far away from any vertex, don't do anything
    if (index_to_remove < 0)
        return true;

    points.erase(points.begin() + index_to_remove);
    ctx_->refresh();

    return true;
}

bool PathControl::handleMouseMove(QMouseEvent *event)
{
    bool event_blocked = false;
    mouse_position_ = event->pos();

    if (vertex_dragged_) {
        QPointF deltaPosition = leaf_->fromSreenSpace(ctx_, mouse_position_) - leaf_->fromSreenSpace(ctx_, previous_mouse_position_);
        std::vector<QPointF>& points = std::dynamic_pointer_cast<Path>(leaf_)->points();
        points[dragged_vertex_index_].rx() += deltaPosition.x();
        points[dragged_vertex_index_].ry() += deltaPosition.y();
        event_blocked = true;
    }

    previous_mouse_position_ = mouse_position_;
    ctx_->refresh();

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
    case Qt::Key_Shift:
        remove_vertex_mode_ = true;
        break;
    }
}

void PathControl::handleKeyReleased(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Control:
        add_vertex_mode_ = false;
        break;
    case Qt::Key_Shift:
        remove_vertex_mode_ = false;
        break;
    }
}
