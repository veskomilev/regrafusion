// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "gfx/leaf.h"

class Rectangle : public Leaf
{
public:
    Rectangle(std::weak_ptr<RgfCtx> ctx, QRectF rectangle, QColor color);

    ~Rectangle();

    static std::shared_ptr<Rectangle> constructNew(std::weak_ptr<RgfCtx> ctx);

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) override;

    static void drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale);

    inline bool isSpawnPoint() override { return false; }

private:
    static constexpr QRectF kDefaultRectangle = QRectF(-10.0f, -10.0f, 20.0f, 20.0f);

    QRectF rectangle_;

    QColor color_;
};

#endif // RECTANGLE_H
