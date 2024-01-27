// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QDrag>

#include "shape_widget_event_filter.h"

#include "displaywidget.h"

shapeWidgetEventFilter::shapeWidgetEventFilter(std::shared_ptr<RgfCtx> rgf_ctx, QAction *action, QWidget *widget, leaf_type_t leaf_type) :
    rgf_ctx_(rgf_ctx),
    action_(action),
    widget_(widget),
    leaf_type_(leaf_type),
    pressed_(false)
{

}

bool shapeWidgetEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (!action_->isEnabled())
        return QObject::eventFilter(obj, event);

    if (event->type() == QEvent::MouseButtonPress) {
        pressed_ = true;

    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);

        if (pressed_ &&
            (mouse_event->pos().x() < 0 ||
            mouse_event->pos().y() < 0 ||
            mouse_event->pos().x() > widget_->rect().width() ||
            mouse_event->pos().y() > widget_->rect().height())) {

                QMimeData *mimeData = new QMimeData;
                mimeData->setData(kRgfMimeType, QByteArray(1, static_cast<char>(leaf_type_)));

                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->exec();

                pressed_ = false;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (pressed_) {
            rgf_ctx_->addShape(leaf_type_);
            pressed_ = false;
        }
    } else {
        return QObject::eventFilter(obj, event);
    }

    return true;
}
