// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

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

class shapeWidgetEventFilter : public QObject
{
    Q_OBJECT
public:
    shapeWidgetEventFilter(std::shared_ptr<RgfCtx> rgf_ctx, QAction *action, QWidget *widget, leaf_type_t leaf_type);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    std::shared_ptr<RgfCtx> rgf_ctx_;
    leaf_type_t leaf_type_;
    QAction *action_;
    QWidget *widget_;
    bool pressed_;
};

#endif // SHAPEWIDGETEVENTFILTER_H
