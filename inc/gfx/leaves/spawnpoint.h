// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "gfx/leaf.h"

class SpawnPoint : public Leaf
{
public:
    SpawnPoint();

    ~SpawnPoint();

    void draw(std::shared_ptr<QPainter> painter) override;

    inline bool isSpawnPoint() override { return true; };
};

#endif // SPAWNPOINT_H