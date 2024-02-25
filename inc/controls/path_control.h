// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef PATH_CONTROL_H
#define PATH_CONTROL_H

#include <QEvent>
#include <QPainter>

#include "common.h"
#include "controls/control.h"

class PathControl : public Control
{
public:
    PathControl();

    ~PathControl();

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth);

    bool handleEvent(QEvent *event);

};

#endif // PATH_CONTROL_H
