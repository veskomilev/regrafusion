// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef PATH_CONTROL_H
#define PATH_CONTROL_H

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "common.h"
#include "controls/control.h"
#include "gfx/leaves/path.h"

typedef struct {
    QPointF a;
    QPointF b;
    uint ind;
} side_t;

class PathControl : public Control
{
public:
    PathControl(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Path> leaf, uint leaf_depth);

    ~PathControl();

    void draw(std::shared_ptr<QPainter> painter);

    bool handleEvent(QEvent *event);

private:
    void drawMoveVertexMode(std::shared_ptr<QPainter> painter, std::vector<QPointF>& points);

    void drawAddVertexMode(std::shared_ptr<QPainter> painter, std::vector<QPointF>& points);

    void drawRemoveVertexMode(std::shared_ptr<QPainter> painter, std::vector<QPointF>& points);

    side_t findClosestSideToCursor(std::vector<QPointF>& points);

    bool handleMouseButtonPress(QMouseEvent *event);

    bool startDraggingVertex(std::vector<QPointF>& points);

    bool addVertex(std::vector<QPointF>& points);

    bool removeVertex(std::vector<QPointF>& points);

    bool handleMouseMove(QMouseEvent *event);

    bool handleMouseButtonRelease(QMouseEvent *event);

    void handleKeyPressed(QKeyEvent *event);

    void handleKeyReleased(QKeyEvent *event);

    QPointF mouse_position_;
    QPointF previous_mouse_position_;

    qreal kRadius = 5.0;
    qreal kPopUpDistance = kRadius * 2;

    bool vertex_dragged_;
    uint dragged_vertex_index_;

    bool add_vertex_mode_;

    bool remove_vertex_mode_;
};

#endif // PATH_CONTROL_H
