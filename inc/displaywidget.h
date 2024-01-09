// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QPoint>
#include <QStatusBar>
#include <QOpenGLWidget>

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

    void setCtx(std::shared_ptr<RgfCtx> ctx) { ctx_ = ctx; };

private:
    void initializeCanvas(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter);

    bool eventFilter(QObject *obj, QEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    // limit view position to mitigate artifacts due to loss of floating point precision
    void limitViewPosition();

    void updateStatus();

    QStatusBar* status_bar_;

    View view_;

    // navigation related
    QPointF drag_start_position_;
    QPointF view_offset_before_drag_start_;
    static constexpr QPointF kMouseClickCorrection = QPointF(-10.0f, -10.0f);

    std::shared_ptr<RgfCtx> ctx_;

    bool draw_window_buffer_;
};

#endif // DISPLAYWIDGET_H
