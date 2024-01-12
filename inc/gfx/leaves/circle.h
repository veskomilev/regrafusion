// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef CIRCLE_H
#define CIRCLE_H

#include "gfx/leaf.h"

class Circle : public Leaf
{
public:
    Circle(std::weak_ptr<RgfCtx> ctx, qreal radius, QColor color);

    ~Circle();

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) override;

    inline bool isSpawnPoint() override { return false; }

private:
    qreal radius_;

    QColor color_;
};

#endif // CIRCLE_H
