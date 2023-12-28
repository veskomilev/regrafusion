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

void SpawnPoint::draw(std::shared_ptr<QPainter> painter)
{
    Leaf::draw(painter);

    painter->setPen(QColor(0, 0, 0, 128));
    painter->drawEllipse(QPointF(0, 0), 3, 3);
}
