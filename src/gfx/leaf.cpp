// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "common.h"
#include "gfx/leaf.h"
#include "gfx/leaves/circle.h"
#include "gfx/leaves/line.h"
#include "gfx/leaves/path.h"
#include "gfx/leaves/rectangle.h"
#include "rgf_ctx.h"

Leaf::Leaf(std::weak_ptr<RgfCtx> ctx, leaf_type_t type) :
    ctx_(ctx),
    color_id_(QColor(255, 255, 255)),
    selected_(false),
    controls_({}),
    type_(type),
    matrix_(QTransform())
{
}

Leaf::~Leaf()
{
    destroyControls();
}

leaf_type_t Leaf::extractType(const QMimeData *mime_data)
{
    if (mime_data == nullptr)
        return leaf_type_t::invalid;

    QByteArray const &data = mime_data->data(kRgfMimeType);

    if (!data.size())
        return leaf_type_t::invalid;

    if (data.front() >= static_cast<char>(leaf_type_t::invalid))
        return leaf_type_t::invalid;

    return static_cast<leaf_type_t>(data.front());
}


void Leaf::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{
    std::shared_ptr<RgfCtx> ctx_p = ctx_.lock();

    if (ctx_p == nullptr)
        return;

    if (!matrix_.isIdentity()) {
        // combine param is true, so combine matrix_ with the existing transformations instead of overwriting them
        painter->setWorldTransform(matrix_, true);
        color_id_painter->setWorldTransform(matrix_, true);
    }

    // TODO: draw the transformation matrix on top of its respective shape, not below it
    if (ctx_p->getMode() != RgfCtx::mode_t::edit || !selected_ || ctx_p->getSelectedLeafDepth() != depth)
        return;

    // in principle the side should be 1, but 1 pixel is too short of a length
    const uint side = 30;

    // draw the transformation matrix
    painter->setPen(QColor(128, 128, 128, 64));
    painter->drawLine(side, 0, side, side);
    painter->drawLine(0, side, side, side);

    painter->setPen(QColor(255, 0, 0, 64));
    painter->drawLine(0, side, 0, 0);

    painter->setPen(QColor(0, 0, 255, 64));
    painter->drawLine(side, 0, 0, 0);
}

void Leaf::drawDragged(std::shared_ptr<QPainter> painter, leaf_type_t leaf_type, QPointF position, qreal scale)
{
    switch(leaf_type) {
    case leaf_type_t::circle:
        Circle::drawDragged(painter, position, scale);
        break;
    case leaf_type_t::line:
        Line::drawDragged(painter, position, scale);
        break;
    case leaf_type_t::path:
        Path::drawDragged(painter, position, scale);
        break;
    case leaf_type_t::rectangle:
        Rectangle::drawDragged(painter, position, scale);
        break;
    default:
        assert(0 && "Invalid leaf type passed!");
    }
}

std::shared_ptr<Leaf> Leaf::constructNew(std::shared_ptr<RgfCtx> ctx, leaf_type_t leaf_type)
{
    std::shared_ptr<Leaf> leaf;
    switch(leaf_type) {
    case leaf_type_t::circle:
        leaf = Circle::constructNew(ctx);
        break;
    case leaf_type_t::line:
        leaf = Line::constructNew(ctx);
        break;
    case leaf_type_t::path:
        leaf = Path::constructNew(ctx);
        break;
    case leaf_type_t::rectangle:
        leaf = Rectangle::constructNew(ctx);
        break;
    default:
        assert(0 && "Invalid leaf type passed!");
    }

    leaf->color_id_ = ctx->leafIdentifier()->registerLeaf(leaf);

    return leaf;
}

bool Leaf::setTransformationMatrix(QTransform matrix)
{
    if (!matrix.isInvertible())
        return false;

    matrix_ = matrix;
    return true;
}

void Leaf::select()
{
    selected_ = true;
    createControls();
}

void Leaf::deselect()
{
    selected_ = false;
    destroyControls();
}

bool Leaf::passthroughEvent(QEvent *event)
{
    for (auto &control : controls_) {
        if (control->handleEvent(event)) {
            return true;
        }
    }

    return false;
}

QPointF Leaf::fromSreenSpace(std::shared_ptr<RgfCtx> ctx, QPointF coordinate)
{
    coordinate = ctx->toSelectedBranchSpace(coordinate / ctx->getView().scale);
    return matrix_.inverted().map(coordinate);
}

void Leaf::translateNatively(QPointF translation)
{
    matrix_.translate(translation.x(), translation.y());
    emit transformedNatively();
}

QColor Leaf::getUniqueColor(uint depth)
{
    QColor result = color_id_;

    // sanity verification
    depth &= 0x3FF;

    result.setRgb(result.rgb() | depth);

    return result;
}

void Leaf::destroyControls()
{
    controls_.clear();
}

void Leaf::drawControls(std::shared_ptr<QPainter> painter)
{
    for (auto &control : controls_) {
        control->draw(painter);
    }
}
