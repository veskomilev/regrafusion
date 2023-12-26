#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "displaywidget.h"

DisplayWidget::DisplayWidget(QWidget* parent)
    : QOpenGLWidget {parent},
      kIdentity(0, 0),
      drag_start_position_(kIdentity),
      view_offset_before_drag_start_(kIdentity)
{
    // initial window size during constructor invocation is miniscule, so set the view offset on first resizeGL() call
    // (which always gets called on start before paintGL())
    view_offset_ = kIdentity;
    parent->installEventFilter(this);
}

void DisplayWidget::initializeGL()
{
}


void DisplayWidget::paintGL()
{
    QPainter painter(this);

    painter.fillRect(QRect(kIdentity, size_), Qt::white);
    painter.setPen(Qt::gray);
    painter.drawRect(1, 0, size_.x() - 1, size_.y() - 1);

    // white background above is at constant relative position - it shouldn't be affected by the matrix
    painter.setWorldMatrixEnabled(true);
    painter.setWorldTransform(QTransform(1, 0, 0, 1, view_offset_.x(), view_offset_.y()));

    int offset_x = -view_offset_.x();
    int offset_y = -view_offset_.y();

    // draw coordinate grid
    painter.setPen(Qt::gray);
    constexpr uint kGridSize = 40U;
    for (int i = kGridSize; i < size_.x() + offset_x; i += kGridSize) {
        painter.drawLine(i, size_.y() + offset_y, i, offset_y);
    }

    for (int i = -kGridSize; i > offset_x; i -= kGridSize) {
        painter.drawLine(i, size_.y() + offset_y, i, offset_y);
    }

    for (int i = kGridSize; i < size_.y() + offset_y; i += kGridSize) {
        painter.drawLine(size_.x() + offset_x, i, offset_x, i);
    }

    for (int i = -kGridSize; i > offset_y; i -= kGridSize) {
        painter.drawLine(size_.x() + offset_x, i, offset_x, i);
    }

    // draw coordinate axes
    painter.setPen(Qt::black);
    painter.drawLine(size_.x() + offset_x, 0, offset_x, 0);
    painter.drawLine(0, size_.y() + offset_y, 0, offset_y);

    // point out the origin
    painter.setPen(Qt::red);
    painter.drawEllipse(kIdentity, 10, 10);
}

void DisplayWidget::resizeGL(int w, int h)
{
    size_ = QPoint(w, h);

    // initial window size during constructor invocation is miniscule, so set the view offset on first resizeGL() call
    // (which always gets called on start before paintGL())
    if (view_offset_ == kIdentity) {
        view_offset_ = QPoint(w / 2, h / 2);
    }
}

void DisplayWidget::resetView()
{
    view_offset_ = QPoint(size_.x() / 2, size_.y() / 2);
    update();
}

bool DisplayWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        drag_start_position_ = mouseEvent->pos();
        view_offset_before_drag_start_ = view_offset_;
        return true;
    }

    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        view_offset_ = view_offset_before_drag_start_ - drag_start_position_ + mouseEvent->pos();
        update();
        return true;
    }

    return false;
}
