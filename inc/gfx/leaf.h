// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef LEAF_H
#define LEAF_H

#include <QColor>
#include <QMimeData>
#include <QPainter>
#include <QTransform>

#include "common.h"

class RgfCtx;

class Leaf
{
public:
    Leaf(std::weak_ptr<RgfCtx> ctx, leaf_type_t type);

    virtual ~Leaf();

    static leaf_type_t extractType(const QMimeData *mime_data);

    virtual void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth);

    static void drawDragged(std::shared_ptr<QPainter> painter, leaf_type_t leaf_type, QPointF position, qreal scale);

    static std::shared_ptr<Leaf> constructNew(std::weak_ptr<RgfCtx> ctx, leaf_type_t leaf_type);

    virtual inline bool isSpawnPoint() = 0;

    bool setTransformationMatrix(QTransform matrix);

    QTransform& matrix() { return matrix_; }

    inline void unapplyLocalTransformations(std::shared_ptr<QPainter> painter) {
        if (!matrix_.isIdentity()) {
            // combine param is true, so combine matrix_ with the existing transformations instead of overwriting them
            painter->setWorldTransform(matrix_.inverted(), true);
        }
    }

    QColor getColorId() const { return color_id_; }

    // TODO: maybe make a factory
    void setColorId(QColor color_id) { color_id_ = color_id; } /// NOTE: should be used only in LeafIdentifier::RegisterLeaf()

    void select() { selected_ = true; }

    void deselect() { selected_ = false; }

    QPointF toLocalSpace(QPointF coordinate);

protected:
    QColor getUniqueColor(uint depth);

    std::weak_ptr<RgfCtx> ctx_;

    QColor color_id_;

    bool selected_;

private:
    // disable copy and assignment ctors
    Leaf(const Leaf&) = delete;
    Leaf& operator=(const Leaf&) = delete;

    leaf_type_t type_;

    QTransform matrix_;
};


#endif // LEAF_H
