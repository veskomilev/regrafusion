// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef CONTROL_H
#define CONTROL_H

#include <QEvent>
#include <QPainter>
#include <QPointF>

#include "common.h"

class RgfCtx;
class Leaf;

class Control
{
public:
    Control(std::weak_ptr<RgfCtx> ctx, std::weak_ptr<Leaf> leaf);

    virtual ~Control();

    leaf_type_t getType() const { return type_; }

    virtual void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) = 0;

    virtual bool handleEvent(QEvent *event) = 0;

protected:
    QPointF mapLeafSpaceToScreenSpace(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, QPointF point, uint depth);

    QPointF mapScreenSpaceToLeafSpace(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, QPointF point, uint depth);

    leaf_type_t type_;

    std::weak_ptr<Leaf> leaf_;

    std::weak_ptr<RgfCtx> ctx_;
};

#endif // CONTROL_H
