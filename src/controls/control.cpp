// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "controls/control.h"
#include "gfx/leaf.h"
#include "rgf_ctx.h"
#include "view.h"

Control::Control(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, uint leaf_depth) :
    type_(leaf_type_t::invalid),
    leaf_(leaf),
    ctx_(ctx),
    leaf_depth_(leaf_depth)
{
}

Control::~Control()
{
}

QTransform Control::calculateTotalLeafTransforamtion()
{
    QTransform tfm = leaf_->matrix();
    QTransform spawn_tfm = ctx_->getSpawnPointTransformation();

    for (uint i = 0; i < leaf_depth_; i++) {
        tfm *= spawn_tfm;
    }

    return tfm;
}

QPointF Control::mapLeafSpaceToScreenSpace(QPointF point)
{
    View view = ctx_->getView();
    return calculateTotalLeafTransforamtion().map(point) * view.scale + view.offset;
}

QPointF Control::mapScreenSpaceToLeafSpace(QPointF point)
{
    View view = ctx_->getView();
    point /= view.scale;
    point -= view.offset / view.scale;
    return calculateTotalLeafTransforamtion().inverted().map(point);
}
