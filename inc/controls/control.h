// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file control.h */

#ifndef CONTROL_H
#define CONTROL_H

#include <QEvent>
#include <QPainter>
#include <QPointF>

#include "common.h"

class RgfCtx;
class Leaf;

//!  An abstract parent class for "controls" (i.e. widgets in the view area) that are used to edit individual leaves.
class Control
{
public:
    //!
    //! \param ctx A pointer to the context
    //! \param leaf The leaf, which will be edited
    //! \param leaf_depth The leaf's depth within its tree
    //!
    Control(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Leaf> leaf, uint leaf_depth);

    virtual ~Control();


    leaf_type_t getType() const { return type_; }

    //!
    //! Draws the control onto the view area
    //!
    //! \param painter A pointer to the user view painter
    //!
    virtual void draw(std::shared_ptr<QPainter> painter) = 0;

    //!
    //! Processes UI events to control the class instance (if applicable)
    //! If an event has been used for the control, it is blocked so that it isn't handled twice (e.g. so that a mouse move event doesn't
    //! both move a leaf and move the view).
    //!
    //! \param event The event to be processed
    //! \return True if the event has been processed and should be blocked
    //!
    virtual bool handleEvent(QEvent *event) = 0;

protected:
    //!
    //! some Calculates the transformation that maps world space to the controlled leaf space
    //!
    //! \return The total transformation
    //!
    QTransform calculateTotalLeafTransforamtion();

    //!
    //! Maps a point from the controlled leaf space to screen space
    //!
    //! \param point The point to be mapped
    //! \return The mapped point
    //!
    QPointF mapLeafSpaceToScreenSpace(QPointF point);

    //!
    //! Maps a point from screen space to the controlled leaf space
    //!
    //! \param point The point to be mapped
    //! \return The mapped point
    //!
    QPointF mapScreenSpaceToLeafSpace(QPointF point);

    //!  The controlled leaf's type
    leaf_type_t type_;

    //!  The controlled leaf
    std::shared_ptr<Leaf> leaf_;

    std::shared_ptr<RgfCtx> ctx_;

    //!  The controlled leaf's depth
    uint leaf_depth_;
};

#endif // CONTROL_H
