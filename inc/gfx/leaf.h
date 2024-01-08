// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef LEAF_H
#define LEAF_H

#include <QColor>
#include <QPainter>
#include <QTransform>

enum class leaf_type_t { spawn_point, circle, line, rectangle, path };

class Leaf
{
public:
    Leaf(leaf_type_t type);

    virtual ~Leaf();

    virtual void draw(std::shared_ptr<QPainter> painter);

    virtual inline bool isSpawnPoint() = 0;

    bool setTransformationMatrix(QTransform matrix);

    QTransform matrix() { return matrix_; }

    inline void unapplyLocalTransformations(std::shared_ptr<QPainter> painter) {
        if (!matrix_.isIdentity()) {
            // combine param is true, so combine matrix_ with the existing transformations instead of overwriting them
            painter->setWorldTransform(matrix_.inverted(), true);
        }
    }

    QColor GetColorId() const { return color_id_; };

    // TODO: maybe make a factory
    void SetColorId(QColor color_id) { color_id_ = color_id; }; /// NOTE: should be used only in LeafIdentifier::RegisterLeaf()

private:
    // disable copy and assignment ctors
    Leaf(const Leaf&) = delete;
    Leaf& operator=(const Leaf&) = delete;

    leaf_type_t type_;

    QTransform matrix_;

    QColor color_id_;
};


#endif // LEAF_H
