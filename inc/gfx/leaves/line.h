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

    static std::shared_ptr<Line> constructNew(std::weak_ptr<RgfCtx> ctx);

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) override;

    static void drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale);

    inline bool isSpawnPoint() override { return false; }

    QLineF getLine() const { return line_; }

    void setLine(QLineF line) { line_ = line; }

    QColor getColor() const { return color_; }

    void setColor(QColor color) { color_ = color; }

private:
    void createControls() override;

    static constexpr QLineF kDefaultLine = QLineF(-10.0f, -10.0f, 10.0f, 10.0f);

    QLineF line_;

    QColor color_;
};

#endif // LINE_H
