// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/leaves/spawnpoint.h"

SpawnPoint::SpawnPoint() :
    Leaf {leaf_type_t::spawn_point}
{
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter)
{
    Leaf::draw(painter, color_id_painter);


    painter->setPen(QColor(0, 0, 0, 128));
    painter->drawEllipse(QPointF(0, 0), 3, 3);

    if (selected_) {
        painter->setPen(QColor(0, 0, 0, 255));
        painter->drawEllipse(QPointF(0, 0), 4, 4);
    }

    color_id_painter->setBrush(color_id_);
    color_id_painter->setPen(QColor(0, 0, 0, 0));
    color_id_painter->drawEllipse(QPointF(0, 0), 3, 3);
}
