// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QTransform>
#include <QPainterPath>

#include "gfx/leaves/line.h"

#include "math_utils.h"
#include "rgf_ctx.h"

Line::Line(std::weak_ptr<RgfCtx> ctx, QLineF line, QColor color) :
    Leaf {ctx, leaf_type_t::line},
    line_(line),
    color_(color)
{

}

Line::~Line()
{

}

std::shared_ptr<Line> Line::constructNew(std::weak_ptr<RgfCtx> ctx)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx.lock();
    assert(ctx_p != nullptr && "A non existant context was accessed");

    // just a wrapper to get to the private ctor
    struct LineCtor : public Line {
        LineCtor(std::weak_ptr<RgfCtx> ctx, QLineF line, QColor color) :
            Line {ctx, line, color} {}
    };

    return std::make_shared<LineCtor>(ctx_p, kDefaultLine, Qt::red);
}

void Line::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    if (ctx_p == nullptr)
        return;


    Leaf::draw(painter, color_id_painter, depth);

    // draw just the outline
    if (selected_ &&
        ctx_p->getMode() == RgfCtx::mode_t::edit &&
        ctx_p->getSelectedLeafDepth() == depth) {
        QPen pen(QColor(0, 0, 0, 255));
        pen.setWidth(2);
        painter->setPen(pen);

        painter->drawLine(line_);
    }

    // reset pen width
    QPen pen(color_);
    pen.setWidth(1);
    painter->setPen(pen);

    painter->drawLine(line_);

    if (ctx_p->getMode() == RgfCtx::mode_t::edit) {

        QPen pen(getUniqueColor(depth));
        pen.setWidth(1 +
                     4.0 /
                             (decomposeMatrix(matrix()).avg_scale *
                                ctx_p->getView().scale)
                     ); // make lines easier to select by scaling up color id area if user view lines are too thin to easily select

        color_id_painter->setPen(pen);
        color_id_painter->drawLine(line_);

        // reset pen width
        pen.setWidth(1);
        color_id_painter->setPen(pen);
    }

    unapplyLocalTransformations(painter);
    unapplyLocalTransformations(color_id_painter);
}

void Line::drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale)
{
    painter->setPen(QColor(0, 0, 0, 255));
    painter->setBrush(QColor(0, 0, 0, 0));

    QTransform t;
    t.scale(1 / scale, 1 / scale);
    t.translate(position.rx(), position.ry());
    painter->setWorldTransform(t, true);
    painter->drawLine(kDefaultLine);
    painter->setWorldTransform(t.inverted(), true);
}

void Line::createControls()
{

}
