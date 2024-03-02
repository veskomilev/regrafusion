// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef CIRCLE_H
#define CIRCLE_H

#include "gfx/leaf.h"

class Circle : public Leaf
{
private:
    Circle(std::weak_ptr<RgfCtx> ctx, qreal radius, QColor color);

public:
    ~Circle();

    static std::shared_ptr<Circle> constructNew(std::weak_ptr<RgfCtx> ctx);

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) override;

    static void drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale);

    inline bool isSpawnPoint() override { return false; }

    qreal getRadius() const { return radius_; }

    void setRadius(qreal radius) { radius_ = radius; }

    QColor getColor() const { return color_; }

    void setColor(QColor color) { color_ = color; }

private:
    void createControls() override;

    static constexpr float kDefaultRadius = 20.0f;

    qreal radius_;

    QColor color_;
};

#endif // CIRCLE_H
