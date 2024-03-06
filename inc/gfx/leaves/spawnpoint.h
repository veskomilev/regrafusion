// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file spawnpoint.h */

#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "gfx/leaf.h"

//!  A Leaf that is a spawn point. Spawn points determine the origin and transformations of the child Branch within the current Branch

//! \sa Branch
class SpawnPoint : public Leaf
{
public:
    //!
    //! \param ctx A pointer to the context
    //!
    SpawnPoint(std::weak_ptr<RgfCtx> ctx);

    ~SpawnPoint();

    //!
    //! Draws a leaf instance onto the view area and the color id buffer
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //! \param color_id_painter A pointer to painter that paints onto the color id buffer
    //! \param depth Which consecutive branch the leaf is on
    //!
    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) override;

    inline bool isSpawnPoint() override { return true; }

private:
    //!
    //! Creates controls (widgets in the view area) to modify the leaf
    //!
    void createControls() override;

};

#endif // SPAWNPOINT_H
