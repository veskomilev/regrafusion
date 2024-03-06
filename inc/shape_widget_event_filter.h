// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file shape_widget_event_filter.h */

#ifndef SHAPEWIDGETEVENTFILTER_H
#define SHAPEWIDGETEVENTFILTER_H

#include <QAction>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QObject>
#include <QWidget>

#include "rgf_ctx.h"
#include "gfx/leaf.h"

//!  A specialised event filter for QAction instances, so that new shapes can be added both by clicking and Drag-and-Dropping
class ShapeWidgetEventFilter : public QObject
{
    Q_OBJECT
public:
    //!
    //! Creates a new event filter
    //!
    //! \param rgf_ctx Pointer to the context
    //! \param action Pointer to the QAction
    //! \param widget Pointer to the QWidget of the QAction
    //! \param leaf_type Type of the shape to be added
    //!
    ShapeWidgetEventFilter(std::shared_ptr<RgfCtx> rgf_ctx, QAction *action, QWidget *widget, leaf_type_t leaf_type);

protected:
    //!
    //! The overridden event handling function
    //!
    //! \param obj Watched object
    //! \param event The event to handle
    //! \return True if the event has been processed and should be blocked from being handled by Qt framework
    //!
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    std::shared_ptr<RgfCtx> rgf_ctx_;

    leaf_type_t leaf_type_;

    QAction *action_;

    QWidget *widget_;

    //!  Whether the QWidget is currently pressed
    bool pressed_;
};

#endif // SHAPEWIDGETEVENTFILTER_H
