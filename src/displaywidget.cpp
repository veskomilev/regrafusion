// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QGuiApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "displaywidget.h"
#include "gfx/leaves/circle.h"
#include "gfx/leaves/line.h"
#include "gfx/leaves/path.h"
#include "gfx/leaves/rectangle.h"
#include "gfx/tree.h"
#include "leaf_identifier.h"
#include "math.h"
#include "rgf_ctx.h"
#include "uipainter.h"

DisplayWidget::DisplayWidget(QWidget* parent) :
    QOpenGLWidget {parent},
    status_bar_(nullptr),
    view_({View::kOffsetIdentity, View::kOffsetIdentity, 1.0}),
    previous_mouse_position_(View::kOffsetIdentity),
    draw_window_buffer_(true),
    dragged_leaf_ {false, leaf_type_t::circle, QPointF(0, 0)},
    mouse_dragged_(false)
{
    // initial window size during constructor invocation is miniscule, so set the view offset on first resizeGL() call
    // (which always gets called on start before paintGL())
    view_.offset = View::kOffsetIdentity;
    parent->installEventFilter(this);
    // TODO: do something about multiscreen support of the window buffer - secondary screen could have a larger resolution
    setAcceptDrops(true);

    setMouseTracking(true);
}

void DisplayWidget::initializeGL()
{
}

void DisplayWidget::paintGL()
{
    std::shared_ptr<QPainter> painter = std::make_shared<QPainter>(ctx_->windowBuffer().get());
    std::shared_ptr<QPainter> color_id_painter = std::make_shared<QPainter>(ctx_->colorIdBuffer().get());

    initializeCanvas(painter, color_id_painter);

    UiPainter uipainter(view_, painter);

    uipainter.drawGridAndAxes();

    uipainter.drawRulerNumbers();

    // above elements are at constant relative position - they shouldn't be affected by the matrix
    // also, grid and axes look better when they're always a single pixel wide
    painter->setWorldMatrixEnabled(true);
    painter->setWorldTransform(QTransform(view_.scale, 0, 0, view_.scale, view_.offset.x(), view_.offset.y()));

    color_id_painter->setWorldMatrixEnabled(true);
    color_id_painter->setWorldTransform(QTransform(view_.scale, 0, 0, view_.scale, view_.offset.x(), view_.offset.y()));

    // draw the tree itself
    TreeStatistics stats = ctx_->tree()->draw(painter, color_id_painter);

    // draw a new DnD leaf
    drawDraggedLeaf(painter);

    // disable the matrix for overlaid elements
    painter->setWorldMatrixEnabled(false);

    // overlay coordinate labels on top of drawn elements
    uipainter.drawCoordinateLabels();

    uipainter.drawStats(stats);

    uipainter.drawCtxMode(ctx_->getMode());

    QPainter display_painter(this);
    if (draw_window_buffer_) {
        display_painter.drawImage(0, 0, *ctx_->windowBuffer());
    } else {
        display_painter.drawImage(0, 0, *ctx_->colorIdBuffer());
    }
}

void DisplayWidget::resizeGL(int w, int h)
{
    view_.size = QPointF(w, h);

    // initial window size during constructor invocation is miniscule, so set the view offset on first resizeGL() call
    // (which always gets called on start before paintGL())
    if (view_.offset == View::kOffsetIdentity) {
        view_.offset = QPointF(w / 2.0f, h / 2.0f);
    }

    limitViewPosition();
    updateStatus();
}

void DisplayWidget::resetViewPosition()
{
    view_.offset = view_.size / 2;
    updateStatus();
    update();
}

void DisplayWidget::resetViewScale()
{
    view_.scale = 1.0f;
    updateStatus();
    update();
}

void DisplayWidget::setStatusBar(QStatusBar * const &bar)
{
    status_bar_ = bar;
    updateStatus();
}

void DisplayWidget::switchBuffers()
{
    draw_window_buffer_ = !draw_window_buffer_;
}

void DisplayWidget::initializeCanvas(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->fillRect(QRectF(View::kOffsetIdentity, view_.size), Qt::white);
    painter->setPen(Qt::gray);
    painter->drawRect(1, 0, view_.size.x() - 1, view_.size.y() - 1);

    color_id_painter->fillRect(QRectF(View::kOffsetIdentity, view_.size), ctx_->leafIdentifier()->getBackgroundColor());
}

bool DisplayWidget::eventFilter(QObject *obj, QEvent *event)
{
    // let the selected leaf's controls make use of the event first
    // and don't handle it twice, if that's the case
    if (ctx_ != nullptr &&
        ctx_->getSelectedLeaf() != nullptr &&
        ctx_->getSelectedLeaf()->passthroughEvent((event))) {
        return true;
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        mouse_dragged_ = true;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        previous_mouse_position_ = mouseEvent->pos();
        QPointF cursor_position = mouseEvent->pos() + kMouseClickCorrection;

        if (ctx_->getMode() == RgfCtx::mode_t::edit) {

            ctx_->tree()->deselect();

            uint leaf_depth = 0;
            auto leaf = ctx_->leafIdentifier()->getLeaf(ctx_->colorIdBuffer(), cursor_position, leaf_depth);
            if (leaf != nullptr) {
                leaf->select();
                ctx_->setSelectedLeaf(leaf, leaf_depth);

            } else {
                ctx_->setSelectedLeaf(nullptr, 0);
            }

        }

        update();

        return true;
    }

    if (event->type() == QEvent::MouseMove && mouse_dragged_)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (ctx_->getMode() == RgfCtx::mode_t::edit && ctx_->getSelectedLeaf() != nullptr) {
            moveLeaf(mouseEvent, ctx_->getSelectedLeaf());
        } else {
            moveView(mouseEvent);
        }

        updateStatus();
        update();
        previous_mouse_position_ = mouseEvent->pos();
        return true;
    }

    if (event->type() == QEvent::MouseButtonRelease)
    {
        mouse_dragged_ = false;
    }

    return false;
}

void DisplayWidget::moveView(QMouseEvent *mouseEvent)
{
    view_.offset += mouseEvent->pos() - previous_mouse_position_;
    limitViewPosition();
}

void DisplayWidget::moveLeaf(QMouseEvent *mouseEvent, std::shared_ptr<Leaf> leaf)
{
    // make spawn points editable only from the base branch
    // this is a task that requires much more complex calculations
    if (leaf->isSpawnPoint() && ctx_->getSelectedLeafDepth() > 0)
        return;

    auto toLeafSpace = [this, leaf](QPointF coords) {
        coords = ctx_->toSelectedBranchSpace(coords / view_.scale);
        return leaf->toLocalSpace(coords);
    };

    QPointF deltaPosition = toLeafSpace(mouseEvent->pos()) - toLeafSpace(previous_mouse_position_);

    leaf->translateNatively(deltaPosition);
}

void DisplayWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        ctx_->switchModesAction();
        event->accept();
    }
}

void DisplayWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();
    float delta_y = numDegrees.y();

    if (delta_y != 0.0f) {
        float old_scale = view_.scale;

        view_.scale *= pow(1.1f, delta_y / 100.0f);
        view_.scale = fmin(View::kMaxScale, view_.scale);
        view_.scale = fmax(View::kMinScale, view_.scale);

        // scale around the window center instead of around the origin - so adjust the view
        float scale_factor = view_.scale / old_scale;
        view_.offset = (view_.offset - view_.size / 2) * scale_factor + view_.size / 2;

        updateStatus();
        update();
    }

    event->accept();
}

void DisplayWidget::limitViewPosition()
{
    view_.offset.rx() = fmin(view_.offset.x(), view_.size.x() / 2 + View::kMaxViewOffset * view_.scale);
    view_.offset.rx() = fmax(view_.offset.x(), view_.size.x() / 2 - View::kMaxViewOffset * view_.scale);
    view_.offset.ry() = fmin(view_.offset.y(), view_.size.y() / 2 + View::kMaxViewOffset * view_.scale);
    view_.offset.ry() = fmax(view_.offset.y(), view_.size.y() / 2 - View::kMaxViewOffset * view_.scale);
}

void DisplayWidget::dragMoveEvent(QDragMoveEvent *event)
{
    dragged_leaf_.exists = false;

    leaf_type_t leaf_type = Leaf::extractType(event->mimeData());
    if (leaf_type == leaf_type_t::invalid) {
        update();
        return;
    }

    dragged_leaf_.exists = true;
    dragged_leaf_.leaf_type = leaf_type;
    dragged_leaf_.position = event->position() - view_.offset;
    update();

    event->acceptProposedAction();
}

void DisplayWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    dragged_leaf_.exists = false;
    update();
}

void DisplayWidget::drawDraggedLeaf(std::shared_ptr<QPainter> painter)
{
    if (!dragged_leaf_.exists)
        return;

    Leaf::drawDragged(painter, dragged_leaf_.leaf_type, dragged_leaf_.position, view_.scale);
}

void DisplayWidget::dropEvent(QDropEvent *event)
{
    dragged_leaf_.exists = false;

    leaf_type_t leaf_type = Leaf::extractType(event->mimeData());
    if (leaf_type == leaf_type_t::invalid) {
        update();
        return;
    }

    ctx_->addShape(leaf_type, event->position() - view_.offset, view_.scale);
    update();
    event->acceptProposedAction();
}

void DisplayWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void DisplayWidget::updateStatus()
{
    assert(status_bar_ != nullptr);
    status_bar_->showMessage(
        QString("Position: (") +
        QString::number(view_.size.x() / view_.scale / 2 - view_.offset.x() / view_.scale) +
        QString(", ") +
        QString::number(view_.size.y() / view_.scale / 2 - view_.offset.y() / view_.scale) +
        QString("); Scale: ") +
        QString::number(view_.scale)
        );
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    eventFilter(this, event);
}
