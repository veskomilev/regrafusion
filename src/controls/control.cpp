// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "controls/control.h"
#include "gfx/leaf.h"
#include "rgf_ctx.h"
#include "view.h"

Control::Control(std::weak_ptr<RgfCtx> ctx, std::weak_ptr<Leaf> leaf) :
    type_(leaf_type_t::invalid),
    leaf_(leaf),
    ctx_(ctx)
{

}

Control::~Control()
{
}

QPointF Control::mapPointToLeafInBranch(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, QPointF point, uint depth)
{
    View view = ctx->getView();
    QTransform tfm = leaf->matrix();
    QTransform spawn_tfm = ctx->getSpawnPointTransformation();

    for (uint i = 0; i < depth; i++) {
        tfm *= spawn_tfm;
    }

    point = tfm.map(point);

    qreal xpos = point.x() * view.scale + view.offset.x();
    qreal ypos = point.y() * view.scale + view.offset.y();

    return QPointF(xpos, ypos);
}

QPointF Control::inverseMapPointToLeafInBranch(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, QPointF point, uint depth)
{
    View view = ctx->getView();
    point /= view.scale;
    point -= view.offset / view.scale;

    QTransform tfm = leaf->matrix();
    QTransform spawn_tfm = ctx->getSpawnPointTransformation();
    for (uint i = 0; i < depth; i++) {
        tfm *= spawn_tfm;
    }

    return tfm.inverted().map(point);
}
