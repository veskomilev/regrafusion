// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef LEAF_H
#define LEAF_H

#include <QColor>
#include <QPainter>
#include <QTransform>

enum class leaf_type_t { spawn_point, circle, line, rectangle, path };

class RgfCtx;

class Leaf
{
public:
    Leaf(std::weak_ptr<RgfCtx> ctx, leaf_type_t type);

    virtual ~Leaf();

    virtual void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth);

    virtual inline bool isSpawnPoint() = 0;

    bool setTransformationMatrix(QTransform matrix);

    QTransform matrix() { return matrix_; }

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
