#ifndef LEAF_H
#define LEAF_H

#include <QTransform>

enum class leaf_type_t { spawn_point, circle };

class Leaf
{
public:
    Leaf(leaf_type_t type, QTransform matrix);

    virtual ~Leaf() = 0;

    virtual void draw() = 0;

private:
    // disable copy and assignment ctors
    Leaf(const Leaf&) = delete;
    Leaf& operator=(const Leaf&) = delete;

    leaf_type_t type_;
    QTransform matrix_;
};

#endif // LEAF_H
