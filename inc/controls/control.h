// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef CONTROL_H
#define CONTROL_H

#include <QEvent>
#include <QPainter>

#include "common.h"
#include "gfx/leaf.h"

class Control
{
public:
    Control();

    virtual ~Control();

    leaf_type_t getType() const { return type_; }

    virtual void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter) = 0;

    virtual bool handleEvent(QEvent *event) = 0;

protected:
    leaf_type_t type_;

};

#endif // CONTROL_H
