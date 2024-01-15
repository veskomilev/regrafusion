// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QPoint>
#include <QStatusBar>
#include <QOpenGLWidget>

#include "gfx/leaf.h"
#include "view.h"

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

private:
    void initializeCanvas(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter);

    bool eventFilter(QObject *obj, QEvent *event) override;

    void moveView(QMouseEvent *mouseEvent);

    void moveLeaf(QMouseEvent *mouseEvent, std::shared_ptr<Leaf> leaf);

    void keyPressEvent(QKeyEvent *event) override;

    void handleKeyTabPressed();

    void handleKeyDeletePressed();

    void wheelEvent(QWheelEvent *event) override;

    // limit view position to mitigate artifacts due to loss of floating point precision
    void limitViewPosition();

    void updateStatus();

    QStatusBar* status_bar_;

    View view_;

    // navigation related
    QPointF previous_mouse_position_;
    static constexpr QPointF kMouseClickCorrection = QPointF(-10.0f, -10.0f);

    std::shared_ptr<RgfCtx> ctx_;

    bool draw_window_buffer_;
};

#endif // DISPLAYWIDGET_H
