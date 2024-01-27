// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef PATH_H
#define PATH_H

#include "gfx/leaf.h"

class Path : public Leaf
{
public:
    Path(std::weak_ptr<RgfCtx> ctx, QColor color);

    ~Path();

    static std::shared_ptr<Path> constructNew(std::weak_ptr<RgfCtx> ctx);

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) override;

    static void drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale);

    inline bool isSpawnPoint() override { return false; }

    void addPoint(QPointF point);

private:
    static const std::vector<QPointF> kDefaultPoints;

    std::vector<QPointF> points_;

    QColor color_;
};

#endif // PATH_H
