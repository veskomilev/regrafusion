// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2024  Vesko Milev

/*! \file math_utils.h
    \brief Contains various math utilities that aren't specific to any class.
*/

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <QPoint>
#include <QPointF>
#include <QTransform>

//!  A struct that describes the transformations which a matrix contains
struct TransformationInfo {
    QPointF location; //!< translation
    qreal rotation_rad; //!< rotation in radians
    qreal rotation_deg; //!< rotation in degrees
    QPointF scale; //!< scaling
    qreal avg_scale; //!< average x and y scale
};

//!
//! Decomposes a transformation matrix into its individual transformation components (translation, rotation, scaling)
//!
//! \param matrix The matrix to be decomposed
//! \return Its individual transformation components
//! \sa TransformationInfo
//!
TransformationInfo decomposeMatrix(QTransform matrix);

// todo: maybe move this to a test framework
//!
//! Testing function
//!
void math_utils_test();

//!
//! Returns the distance between two points with floating point coordinates
//!
//! \param p1 The first point
//! \param p2 The second point
//! \return The distance between them
//!
inline qreal getPointDistance(QPointF p1, QPointF p2)
{
    qreal x = p1.rx() - p2.rx();
    qreal y = p1.ry() - p2.ry();
    return sqrt(x * x + y * y);
}

//!
//! Returns the distance between two points with integer point coordinates
//!
//! \param p1 The first point
//! \param p2 The second point
//! \return The distance between them
//!
inline qreal getPointDistance(QPoint p1, QPoint p2)
{
    return getPointDistance(p1.toPointF(), p2.toPointF());
}

//!
//! Calculates the cross product of two vectors, both starting at (0, 0)
//!
//! \param v1 The first vector
//! \param v2 The second vector
//! \return The cross product between them
//!
inline qreal crossProduct(QPointF v1, QPointF v2)
{
    return v1.x() * v2.y() - v1.y() * v2.x();
}

#endif // MATH_UTILS_H
