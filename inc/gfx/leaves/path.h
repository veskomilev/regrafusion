// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef PATH_H
#define PATH_H

#include "gfx/leaf.h"

class Path : public Leaf
{
public:
    Path(QColor color);

    ~Path();

    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter) override;

    inline bool isSpawnPoint() override { return false; };

    void addPoint(QPointF point);

private:
    std::vector<QPointF> points_;

    QColor color_;
};

#endif // PATH_H
