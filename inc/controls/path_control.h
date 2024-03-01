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

class PathControl : public Control
{
public:
    PathControl(std::weak_ptr<RgfCtx> ctx, std::weak_ptr<Leaf> leaf);

    ~PathControl();

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth);

    bool handleEvent(QEvent *event);

private:
    bool handleMouseButtonPress(QMouseEvent *event);

    bool handleMouseMove(QMouseEvent *event);

    bool handleMouseButtonRelease(QMouseEvent *event);

    QPointF mouse_position_;
    QPointF previous_mouse_position_;

    qreal kRadius = 5.0;
    qreal kPopUpDistance = kRadius * 2;

    bool vertex_dragged_;
    uint dragged_vertex_index_;
};

#endif // PATH_CONTROL_H
