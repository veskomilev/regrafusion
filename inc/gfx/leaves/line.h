// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef LINE_H
#define LINE_H

#include "gfx/leaf.h"

class Line : public Leaf
{
public:
    Line(std::weak_ptr<RgfCtx> ctx, QLineF line, QColor color);

    ~Line();

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter) override;

    inline bool isSpawnPoint() override { return false; };

private:
    QLineF line_;

    QColor color_;
};

#endif // LINE_H
