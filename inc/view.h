// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file view.h */

#ifndef VIEW_H
#define VIEW_H

#include <QPointF>

//!  A class representing the current position of the viewport within the world space, i.e. the relation between world space and view space
struct View
{
    //!  Size of the view area (viewport)
    QPointF size;

    //!  Offset (translation) of the view space with regards to the world space
    QPointF offset;

    //!  Scaling of the view space
    float scale;

    //!  Origin point of the space (translation identity)
    static constexpr QPointF kOffsetIdentity = QPointF(0.0f, 0.0f);

    //!  Maximum distance from the origin along either coordinate
    static constexpr float kMaxViewOffset = 5000.0f;

    static constexpr float kMinScale = 0.2f;

    static constexpr float kMaxScale = 100.0f;

    //! GOTCHA: for some reason, sometimes the click coordinates are offset by a constant amount from the very tip of the cursor.
    //! This might have to do with the cursor icon and/or OS.
    //! TODO: figure out where this comes from and address it in a more appropriate way
    static constexpr QPointF kMouseClickCorrection = QPointF(-10.0f, -10.0f);
};

#endif // VIEW_H
