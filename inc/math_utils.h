// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2024  Vesko Milev

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <QTransform>

struct TransformationInfo {
    QPointF location;
    qreal rotation_rad;
    qreal rotation_deg;
    QPointF scale;
};

TransformationInfo decomposeMatrix(QTransform matrix)
{
    TransformationInfo info;
    info.location.rx() = matrix.dx();
    info.location.ry() = matrix.dy();

    QPointF mx(matrix.m11(), matrix.m21());
    QPointF my(matrix.m12(), matrix.m22());

    info.scale.rx() = sqrt(mx.rx() * mx.rx() + mx.ry() * mx.ry());
    info.scale.ry() = sqrt(my.rx() * my.rx() + my.ry() * my.ry());

    qreal normalised_m11 = matrix.m11() / info.scale.rx();
    info.rotation_rad = acos(normalised_m11);
    info.rotation_deg = 180 * info.rotation_rad / M_PI;

    return info;
}

// todo: maybe move this to a test framework
void math_utils_test()
{
    QTransform test_matrix;
    test_matrix.scale(1.2, 1);
    test_matrix.scale(1, 1.4);
    test_matrix.rotate(20);
    test_matrix.scale(2, 2);
    test_matrix.rotate(50);

    auto info = decomposeMatrix(test_matrix);
    assert(info.rotation_deg == 70);
    assert(info.scale.rx() == 2.4);
    assert(info.scale.ry() == 2.8);
}

#endif // MATH_UTILS_H