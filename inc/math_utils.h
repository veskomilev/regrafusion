// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2024  Vesko Milev

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <QPoint>
#include <QPointF>
#include <QTransform>

struct TransformationInfo {
    QPointF location;
    qreal rotation_rad;
    qreal rotation_deg;
    QPointF scale;
};

TransformationInfo decomposeMatrix(QTransform matrix);

// todo: maybe move this to a test framework
void math_utils_test();

inline qreal getPointDistance(QPointF p1, QPointF p2)
{
    qreal x = p1.rx() - p2.rx();
    qreal y = p1.ry() - p2.ry();
    return sqrt(x * x + y * y);
}

inline qreal getPointDistance(QPoint p1, QPoint p2)
{
    return getPointDistance(p1.toPointF(), p2.toPointF());
}

// calculates the cross product of two vectors, both starting at (0, 0)
inline qreal crossProduct(QPointF v1, QPointF v2)
{
    return v1.x() * v2.y() - v1.y() * v2.x();
}

#endif // MATH_UTILS_H
