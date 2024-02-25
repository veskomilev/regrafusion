// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include "gfx/leaves/spawnpoint.h"
#include "rgf_ctx.h"

SpawnPoint::SpawnPoint(std::weak_ptr<RgfCtx> ctx) :
    Leaf {ctx, leaf_type_t::spawn_point}
{
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    if (ctx_p == nullptr)
        return;


    Leaf::draw(painter, color_id_painter, depth);


    painter->setPen(QColor(0, 0, 0, 128));
    painter->drawEllipse(QPointF(0, 0), 3, 3);

    if (selected_ && ctx_p->getMode() == RgfCtx::mode_t::edit) {
        painter->setPen(QColor(0, 0, 0, 255));
        painter->drawEllipse(QPointF(0, 0), 4, 4);
    }

    if (ctx_p->getMode() == RgfCtx::mode_t::edit) {
        color_id_painter->setBrush(getUniqueColor(depth));
        color_id_painter->setPen(QColor(0, 0, 0, 0));
        color_id_painter->drawEllipse(QPointF(0, 0), 3, 3);
    }

    drawControls(painter, color_id_painter, depth);
}

void SpawnPoint::createControls()
{

}
