// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef VIEW_H
#define VIEW_H

#include <QPointF>

struct View
{
    QPointF size;
    QPointF offset;
    float scale;

    static constexpr QPointF kOffsetIdentity = QPointF(0.0f, 0.0f);
    static constexpr float kMaxViewOffset = 5000.0f;

    static constexpr float kMinScale = 0.2f;
    static constexpr float kMaxScale = 100.0f;
};

#endif // VIEW_H
