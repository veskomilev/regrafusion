// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "gfx/leaf.h"

class Rectangle : public Leaf
{
public:
    Rectangle(QRectF rectangle, QColor color);

    ~Rectangle();

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter) override;

    inline bool isSpawnPoint() override { return false; };

private:
    QRectF rectangle_;

    QColor color_;
};

#endif // RECTANGLE_H
