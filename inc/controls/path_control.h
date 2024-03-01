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

typedef struct {
    QPointF a;
    QPointF b;
    uint ind;
} side_t;

class PathControl : public Control
{
public:
    PathControl(std::weak_ptr<RgfCtx> ctx, std::weak_ptr<Leaf> leaf);

    ~PathControl();

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth);

    bool handleEvent(QEvent *event);

private:
    void drawMoveVertexMode(std::shared_ptr<QPainter> painter, std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points, uint depth);

    void drawAddVertexMode(std::shared_ptr<QPainter> painter, std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points, uint depth);

    void drawRemoveVertexMode(std::shared_ptr<QPainter> painter, std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points, uint depth);

    side_t findClosestSideToCursor(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points, uint depth);

    bool handleMouseButtonPress(QMouseEvent *event);

    bool startDraggingVertex(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points);

    bool addVertex(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points);

    bool removeVertex(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, std::vector<QPointF>& points);

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
