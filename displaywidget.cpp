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

    initializeCanvas(&painter);

    // set up and adequate grid scale
    float grid_size = kGridSize * view_scale_;
    float ruler_size = kGridSize;
    scaleGridSizes(grid_size, ruler_size);

    drawGridAndAxes(&painter, grid_size);

    // above elements are at constant relative position - they shouldn't be affected by the matrix
    // also, grid and axes look better when they're always a single pixel wide
    painter.setWorldMatrixEnabled(true);
    painter.setWorldTransform(QTransform(view_scale_, 0, 0, view_scale_, view_offset_.x(), view_offset_.y()));

    // point out the origin
    painter.setPen(Qt::red);
    painter.drawEllipse(kViewIdentity, 1, 1);
    painter.drawEllipse(kViewIdentity, 10, 10);
    painter.drawEllipse(kViewIdentity, 100, 100);
    painter.drawEllipse(kViewIdentity, 1000, 1000);

    // disable the matrix for overlaid elements
    painter.setWorldMatrixEnabled(false);

    // overlay coordinate labels on top of drawn elements
    // TODO: make it a setting if labels and/or axes should be overlaid or not
    drawCoordinateLabels(&painter);

    drawRulerNumbers(&painter, grid_size, ruler_size);
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

void DisplayWidget::resetViewPosition()
{
    view_offset_ = window_size_ / 2;
    updateStatus();
    update();
}

void DisplayWidget::resetViewScale()
{
    view_scale_ = 1.0f;
    updateStatus();
    update();
}

void DisplayWidget::setStatusBar(QStatusBar * const &bar)
{
    status_bar_ = bar;
    updateStatus();
}

void DisplayWidget::initializeCanvas(QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->fillRect(QRectF(kViewIdentity, window_size_), Qt::white);
    painter->setPen(Qt::gray);
    painter->drawRect(1, 0, window_size_.x() - 1, window_size_.y() - 1);
}

void DisplayWidget::scaleGridSizes(float& grid_size, float& ruler_size)
{
    // hardcoded values for precise control over scaling
    if (grid_size < 2) {
        grid_size *= 100;
        ruler_size = 10'000;
    } else if (grid_size < 2.5) {
        grid_size *= 40;
        ruler_size = 4000;
    } else if (grid_size < 5) {
        grid_size *= 20;
        ruler_size = 2000;
    } else if (grid_size < 20) {
        grid_size *= 10;
        ruler_size = 1000;
    } else if (grid_size < 25) {
        grid_size *= 4;
        ruler_size = 400;
    } else if (grid_size < 50) {
        grid_size *= 2;
        ruler_size = 200;
    } else if (grid_size < 200) {
        // do nothing - identity
    } else if (grid_size < 500) {
        grid_size /= 5;
        ruler_size = 20;
    } else if (grid_size < 1000) {
        grid_size /= 10;
        ruler_size = 10;
    } else if (grid_size < 2000) {
        grid_size /= 20;
        ruler_size = 5;
    } else if (grid_size < 5000) {
        grid_size /= 50;
        ruler_size = 2;
    } else if (grid_size < 20000) {
        grid_size /= 100;
        ruler_size = 1;
    } else {
        grid_size /= 500;
        ruler_size = 0.2;
    }
}

void DisplayWidget::drawGridAndAxes(QPainter *painter, float grid_size)
{
    // draw coordinate grid
    painter->setPen(Qt::gray);
    for (float i = grid_size; i < window_size_.x() - view_offset_.x(); i += grid_size) {
        painter->drawLine(i + view_offset_.x(), 0, i + view_offset_.x(), window_size_.y());
    }

    for (float i = -grid_size; i > -view_offset_.x(); i -= grid_size) {
        painter->drawLine(i + view_offset_.x(), 0, i + view_offset_.x(), window_size_.y());
    }

    for (float i = grid_size; i < window_size_.y() - view_offset_.y(); i += grid_size) {
        painter->drawLine(0, i + view_offset_.y(), window_size_.x(), i + view_offset_.y());
    }

    for (float i = -grid_size; i > -view_offset_.y(); i -= grid_size) {
        painter->drawLine(0, i + view_offset_.y(), window_size_.x(), i + view_offset_.y());
    }

    // draw coordinate axes
    painter->setPen(Qt::black);
    painter->drawLine(0, view_offset_.y(), window_size_.x(), view_offset_.y());
    painter->drawLine(view_offset_.x(), 0, view_offset_.x(), window_size_.y());
}

void DisplayWidget::drawCoordinateLabels(QPainter *painter)
{
    float xpos;
    float ypos;
    painter->setPen(Qt::black);

    // use min/max to make labels stick to edges of the screen
    xpos = window_size_.x() - kLabelsOffset;
    ypos = view_offset_.y() + kLabelsOffset;
    ypos = fmin(ypos, window_size_.y() - kTextHeight);
    ypos = fmax(ypos, kTextHeight);
    xpos = fmax(xpos, view_offset_.x() + kLabelsOffset);
    painter->drawText(xpos, ypos, "x");

    xpos = kLabelsOffset / 2;
    ypos = view_offset_.y() + kLabelsOffset;
    ypos = fmin(ypos, window_size_.y() - kTextHeight);
    ypos = fmax(ypos, kTextHeight);
    xpos = fmin(xpos, view_offset_.x() - kLabelsOffset);
    painter->drawText(xpos, ypos, "-x");

    xpos = view_offset_.x() - kLabelsOffset / 2;
    ypos = window_size_.y() - kTextHeight;
    xpos = fmin(xpos, window_size_.x() - kTextHeight);
    xpos = fmax(xpos, kTextHeight);
    ypos = fmax(ypos, view_offset_.y() + kLabelsOffset);
    painter->drawText(
        QRectF(QPointF(0, ypos),
               QPointF(xpos, ypos + kTextHeight)),
        Qt::AlignRight,
        "y");

    xpos = view_offset_.x() - kLabelsOffset / 2;
    ypos = 0;
    xpos = fmin(xpos, window_size_.x() - kTextHeight);
    xpos = fmax(xpos, kTextHeight);
    ypos = fmin(ypos, view_offset_.y() - kLabelsOffset);
    painter->drawText(
        QRectF(QPointF(0, ypos),
               QPointF(xpos, ypos + kTextHeight)),
        Qt::AlignRight,
        "-y");
}

void DisplayWidget::drawRulerNumbers(QPainter *painter, float grid_size, float ruler_size)
{
    painter->setPen(Qt::black);

    // use min/max to make labels stick to edges of the screen
    for (float i = ruler_size; i * view_scale_ < window_size_.x() - view_offset_.x() - kRulerMarginRight; i += ruler_size) {
        float xpos = i * view_scale_ + view_offset_.x();
        float ypos = view_offset_.y() + kLabelsOffset;
        if (xpos < kRulerMarginLeft)
            continue;

        ypos = fmin(ypos, window_size_.y() - kTextHeight);
        ypos = fmax(ypos, kTextHeight);

        painter->drawText(xpos, ypos, QString::number(i));
    }

    for (float i = -ruler_size; i * view_scale_ >  - view_offset_.x() + kRulerMarginLeft; i -= ruler_size) {
        float xpos = i * view_scale_ + view_offset_.x();
        float ypos = view_offset_.y() + kLabelsOffset;
        if (xpos > window_size_.x() - kRulerMarginRight)
            continue;

        ypos = fmin(ypos, window_size_.y() - kTextHeight);
        ypos = fmax(ypos, kTextHeight);

        painter->drawText(xpos, ypos, QString::number(i));
    }

    for (float i = ruler_size; i * view_scale_ < window_size_.y() - view_offset_.y() - kRulerMarginBottom; i += ruler_size) {
        float ypos = i * view_scale_ + view_offset_.y();
        float xpos = view_offset_.x() - kLabelsOffset / 2;
        if (ypos < kRulerMarginTop)
            continue;

        xpos = fmin(xpos, window_size_.x() - kTextHeight);
        xpos = fmax(xpos, kRulerTextWidth);

        painter->drawText(
            QRectF(QPointF(xpos - window_size_.x(), ypos),
                   QPointF(xpos, ypos + kTextHeight)),
            Qt::AlignRight,
            QString::number(i));
    }

    for (float i = -ruler_size; i * view_scale_ >  - view_offset_.y() + kRulerMarginTop; i -= ruler_size) {
        float ypos = i * view_scale_ + view_offset_.y();
        float xpos = view_offset_.x() - kLabelsOffset / 2;
        if (ypos > window_size_.y() - kRulerMarginTop)
            continue;

        xpos = fmin(xpos, window_size_.x() - kTextHeight);
        xpos = fmax(xpos, kRulerTextWidth);

        painter->drawText(
            QRectF(QPointF(xpos - window_size_.x(), ypos),
                   QPointF(xpos, ypos + kTextHeight)),
            Qt::AlignRight,
            QString::number(i));
    }

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
