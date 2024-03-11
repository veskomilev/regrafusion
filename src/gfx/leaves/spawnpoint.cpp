// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "gfx/leaves/spawnpoint.h"
#include "rgf_ctx.h"

SpawnPoint::SpawnPoint(std::weak_ptr<RgfCtx> ctx) :
    Leaf {ctx, leaf_type_t::spawn_point}
{
}

std::shared_ptr<SpawnPoint> SpawnPoint::constructNew(std::weak_ptr<RgfCtx> ctx)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx.lock();
    assert(ctx_p != nullptr && "A non existant context was accessed");

    // just a wrapper to get to the private ctor
    struct SpawnPointCtor : public SpawnPoint {
        SpawnPointCtor(std::weak_ptr<RgfCtx> ctx) :
            SpawnPoint {ctx} {}
    };

    return std::make_shared<SpawnPointCtor>(ctx_p);
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    Leaf::draw(painter, color_id_painter, depth);

    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();
    if (ctx_p == nullptr)
        return;

    // don't draw spawn points in view mode
    if (ctx_p->getMode() == RgfCtx::mode_t::view)
        return;

    painter->setBrush(Qt::transparent);

    // only highlight the editable spawn point instance, so as not to clutter the viewport
    bool editable = (depth == 0);

    if (editable) {
        painter->setPen(QColor(100, 0, 0, 160));
        painter->drawEllipse(QPointF(0, 0), 3, 3);
    } else {
        painter->setPen(QColor(0, 0, 0, 128));
        painter->drawEllipse(QPointF(0, 0), 2, 2);
    }

    if (selected_ && ctx_p->getMode() == RgfCtx::mode_t::edit && editable) {
        painter->setPen(QColor(0, 0, 0, 255));
        painter->drawEllipse(QPointF(0, 0), 4, 4);
    }

    if (ctx_p->getMode() == RgfCtx::mode_t::edit && editable) {
        color_id_painter->setBrush(getUniqueColor(depth));
        color_id_painter->setPen(QColor(0, 0, 0, 0));
        color_id_painter->drawEllipse(QPointF(0, 0), 3, 3);
    }
}

void SpawnPoint::createControls()
{

}
