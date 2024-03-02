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
    Control(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, uint leaf_depth);

    virtual ~Control();

    leaf_type_t getType() const { return type_; }

    virtual void draw(std::shared_ptr<QPainter> painter) = 0;

    virtual bool handleEvent(QEvent *event) = 0;

protected:
    QTransform calculateTotalLeafTransforamtion();

    QPointF mapLeafSpaceToScreenSpace(QPointF point);

    QPointF mapScreenSpaceToLeafSpace(QPointF point);

    leaf_type_t type_;

    std::shared_ptr<Leaf> leaf_;

    std::shared_ptr<RgfCtx> ctx_;

    uint leaf_depth_;
};

#endif // CONTROL_H
