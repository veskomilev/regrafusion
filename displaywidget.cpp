#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "displaywidget.h"
#include "math.h"

DisplayWidget::DisplayWidget(QWidget* parent) :
    QOpenGLWidget {parent},
    status_bar_(nullptr),
    kViewIdentity(0.0f, 0.0f),
    drag_start_position_(kViewIdentity),
    view_offset_before_drag_start_(kViewIdentity),
    view_scale_(1.0f),
    kMinViewScale(0.01f),
    kMaxViewScale(100.0f)
{
    // initial window size during constructor invocation is miniscule, so set the view offset on first resizeGL() call
    // (which always gets called on start before paintGL())
    view_offset_ = kViewIdentity;
    parent->installEventFilter(this);
}

void DisplayWidget::initializeGL()
{
}


void DisplayWidget::paintGL()
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.fillRect(QRectF(kViewIdentity, window_size_), Qt::white);
    painter.setPen(Qt::gray);
    painter.drawRect(1, 0, window_size_.x() - 1, window_size_.y() - 1);

    float offset_x = view_offset_.x();
    float offset_y = view_offset_.y();
    float window_size_x = window_size_.x();
    float window_size_y = window_size_.y();
    float grid_size = kGridSize * view_scale_;

    // scale up or down grid adequately
    while (grid_size < 25) {
        grid_size *= 5;
    }
    while (grid_size > 125) {
        grid_size /= 5;
    }

    // draw coordinate grid
    painter.setPen(Qt::gray);
    for (float i = grid_size; i < window_size_x - offset_x; i += grid_size) {
        painter.drawLine(i + offset_x, 0, i + offset_x, window_size_y);
    }

    for (float i = -grid_size; i > -offset_x; i -= grid_size) {
        painter.drawLine(i + offset_x, 0, i + offset_x, window_size_y);
    }

    for (float i = grid_size; i < window_size_y - offset_y; i += grid_size) {
        painter.drawLine(0, i + offset_y, window_size_x, i + offset_y);
    }

    for (float i = -grid_size; i > -offset_y; i -= grid_size) {
        painter.drawLine(0, i + offset_y, window_size_x, i + offset_y);
    }

    // draw coordinate axes
    painter.setPen(Qt::black);
    painter.drawLine(0, offset_y, window_size_x, offset_y);
    painter.drawLine(offset_x, 0, offset_x, window_size_y);

    // white background above is at constant relative position - it shouldn't be affected by the matrix
    // also, grid and axes look better when they're always a single pixel wide
    painter.setWorldMatrixEnabled(true);
    painter.setWorldTransform(QTransform(view_scale_, 0, 0, view_scale_, view_offset_.x(), view_offset_.y()));

    // point out the origin
    painter.setPen(Qt::red);
    painter.drawEllipse(kViewIdentity, 1, 1);
    painter.drawEllipse(kViewIdentity, 10, 10);
    painter.drawEllipse(kViewIdentity, 100, 100);
    painter.drawEllipse(kViewIdentity, 1000, 1000);

}

void DisplayWidget::resizeGL(int w, int h)
{
    window_size_ = QPointF(w, h);

    // initial window size during constructor invocation is miniscule, so set the view offset on first resizeGL() call
    // (which always gets called on start before paintGL())
    if (view_offset_ == kViewIdentity) {
        view_offset_ = QPointF(w / 2.0f, h / 2.0f);
    }

    updateStatus();
}

void DisplayWidget::setStatusBar(QStatusBar * const &bar)
{
    status_bar_ = bar;
    updateStatus();
}

void DisplayWidget::resetViewScale()
{
    view_scale_ = 1.0f;
    updateStatus();
    update();
}

void DisplayWidget::resetViewPosition()
{
    view_offset_ = window_size_ / 2;
    updateStatus();
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
        updateStatus();
        update();
        return true;
    }

    return false;
}

void DisplayWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();
    float delta_y = numDegrees.y();

    if (delta_y != 0.0f) {
        float old_scale = view_scale_;

        view_scale_ *= pow(1.1f, delta_y / 100.0f);
        view_scale_ = fmin(kMaxViewScale, view_scale_);
        view_scale_ = fmax(kMinViewScale, view_scale_);

        // scale around the window center instead of around the origin - so adjust the view
        float scale_factor = view_scale_ / old_scale;
        view_offset_ = (view_offset_ - window_size_ / 2) * scale_factor + window_size_ / 2;

        updateStatus();
        update();
    }

    event->accept();
}

void DisplayWidget::updateStatus()
{
    assert(status_bar_ != nullptr);
    status_bar_->showMessage(
        QString("Position: (") +
        QString::number(window_size_.x() / view_scale_ / 2 - view_offset_.x() / view_scale_) +
        QString(", ") +
        QString::number(window_size_.y() / view_scale_ / 2 - view_offset_.y() / view_scale_) +
        QString("); Scale: ") +
        QString::number(view_scale_)
        );
}
