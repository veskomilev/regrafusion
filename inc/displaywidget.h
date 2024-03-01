// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPoint>
#include <QStatusBar>
#include <QOpenGLWidget>

#include "gfx/leaf.h"
#include "view.h"

struct dragged_leaf_t
{
    bool exists;
    leaf_type_t leaf_type;
    QPointF position;
};

class RgfCtx;

class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    DisplayWidget(QWidget* parent = NULL);

    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int w, int h) override;

    void resetViewPosition();

    void resetViewScale();

    void setStatusBar(QStatusBar* const& bar);

    void switchBuffers();

    void setPaintingCtx();

    void setCtx(std::shared_ptr<RgfCtx> ctx) { ctx_ = ctx; }

    void updateStatus();

    View getView() const { return view_; }

    void mouseMoveEvent(QMouseEvent *event);

private:
    void initializeCanvas(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter);

    bool eventFilter(QObject *obj, QEvent *event) override;

    void moveView(QMouseEvent *mouseEvent);

    void moveLeaf(QMouseEvent *mouseEvent, std::shared_ptr<Leaf> leaf);

    void keyPressEvent(QKeyEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    // limit view position to mitigate artifacts due to loss of floating point precision
    void limitViewPosition();

    void dropEvent(QDropEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dragLeaveEvent(QDragLeaveEvent *event);

    void drawDraggedLeaf(std::shared_ptr<QPainter> painter);

    QStatusBar* status_bar_;

    View view_;

    // navigation related
    QPointF previous_mouse_position_;

    std::shared_ptr<RgfCtx> ctx_;

    bool draw_window_buffer_;

    dragged_leaf_t dragged_leaf_;

    bool mouse_dragged_;
};

#endif // DISPLAYWIDGET_H
