// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef LEAF_H
#define LEAF_H

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

private:
    // disable copy and assignment ctors
    Leaf(const Leaf&) = delete;
    Leaf& operator=(const Leaf&) = delete;

    leaf_type_t type_;

    QTransform matrix_;
};


#endif // LEAF_H
