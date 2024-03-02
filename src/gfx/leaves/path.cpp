// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 20232-2024  Vesko Milev

#include <QPainterPath>

#include "controls/path_control.h"
#include "gfx/leaves/path.h"
#include "rgf_ctx.h"

const std::vector<QPointF> Path::kDefaultPoints = {QPointF(0.0f, -20.0f), QPointF(20.0f, 20.0f), QPointF(-20.0f, 20.0f)};

Path::Path(std::weak_ptr<RgfCtx> ctx, QColor color) :
    Leaf {ctx, leaf_type_t::path},
    color_(color)
{

}

Path::~Path()
{

}

std::shared_ptr<Path> Path::constructNew(std::weak_ptr<RgfCtx> ctx)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx.lock();
    assert(ctx_p != nullptr && "A non existant context was accessed");

    // just a wrapper to get to the private ctor
    struct PathCtor : public Path {
        PathCtor(std::weak_ptr<RgfCtx> ctx, QColor color) :
            Path {ctx, color} {}
    };

    auto path = std::make_shared<PathCtor>(ctx_p, Qt::red);
    for (const QPointF& point : kDefaultPoints) {
        path->addPoint(point);
    }

    path->self_ref_ = path;

    return path;
}

void Path::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    if (ctx_p == nullptr)
        return;


    Leaf::draw(painter, color_id_painter, depth);

    if (points_.size() > 0) {
        QPainterPath path(points_[0]);

        for (QPointF& point : points_) {
            path.lineTo(point);
        }

        path.closeSubpath();

        if (selected_ &&
            ctx_p->getMode() == RgfCtx::mode_t::edit &&
            ctx_p->getSelectedLeafDepth() == depth) {
            painter->setPen(QColor(0, 0, 0, 255));
        } else {
            painter->setPen(QColor(0, 0, 0, 0));
        }
        painter->setBrush(color_);
        painter->drawPath(path);

        if (ctx_p->getMode() == RgfCtx::mode_t::edit) {
            color_id_painter->setPen(QColor(0, 0, 0, 0));
            color_id_painter->setBrush(getUniqueColor(depth));
            color_id_painter->drawPath(path);
        }
    }

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}

void Path::drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale)
{
    painter->setPen(QColor(0, 0, 0, 255));
    painter->setBrush(QColor(0, 0, 0, 0));

    QTransform t;
    t.scale(1 / scale, 1 / scale);
    t.translate(position.rx(), position.ry());
    painter->setWorldTransform(t, true);

    QPainterPath path(kDefaultPoints[0]);
    for (const QPointF& point : kDefaultPoints) {
        path.lineTo(point);
    }
    path.closeSubpath();
    painter->drawPath(path);

    painter->setWorldTransform(t.inverted(), true);
}

void Path::addPoint(QPointF point)
{
    points_.push_back(point);
}

void Path::select()
{
    Leaf::select();

    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();
    if (ctx_p == nullptr)
        return;
    ctx_p->setStatusBarMessage("Add vertex: Ctrl + LMB    |    Delete vertex: Shift + LMB");
}

void Path::createControls()
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();
    if (ctx_p == nullptr)
        return;

    std::shared_ptr<Leaf> leaf_p = self_ref_.lock();
    if (leaf_p == nullptr)
        return;

    std::shared_ptr<Path> path_p = std::dynamic_pointer_cast<Path>(leaf_p);

    controls_.push_back(std::make_shared<PathControl>(ctx_p, path_p, ctx_p->getSelectedLeafDepth()));
}
