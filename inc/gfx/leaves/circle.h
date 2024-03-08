// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file circle.h */

#ifndef CIRCLE_H
#define CIRCLE_H

#include "gfx/leaf.h"

//!  A Leaf that is a circle
class Circle : public Leaf
{
private:
    //!
    //! New leaves are meant to be constructed only by calling the factory method constructNew().
    //!
    //! \param ctx A pointer to the context
    //! \param radius The radius of the circle
    //! \param color The color of the circle
    //! \sa constructNew()
    //!
    Circle(std::weak_ptr<RgfCtx> ctx, qreal radius, QColor color);

public:
    ~Circle();

    //!
    //! Factory method for instantiating new leaves
    //!
    //! \param ctx A pointer to the context
    //! \return A pointer to the newly created leaf
    //!
    static std::shared_ptr<Circle> constructNew(std::weak_ptr<RgfCtx> ctx);

    //!
    //! Draws a leaf instance onto the view area and the color id buffer
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //! \param color_id_painter A pointer to painter that paints onto the color id buffer
    //! \param depth Which consecutive branch the leaf is on
    //!
    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth) override;

    //!
    //! Draws a ghost shape under the cursor when a Drag-and-Drop event is occurring to visualise where exactly the new leaf would be
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //! \param position Where to draw the ghost shape
    //! \param scale The scale of the ghost shape
    //!
    static void drawDragged(std::shared_ptr<QPainter> painter, QPointF position, qreal scale);

    inline bool isSpawnPoint() override { return false; }

    qreal getRadius() const { return radius_; }

    void setRadius(qreal radius) { radius_ = radius; }

    QColor getColor() const { return color_; }

    void setColor(QColor color) { color_ = color; }

private:
    //!
    //! Creates controls (widgets in the view area) to modify the leaf
    //!
    void createControls() override;

    //!  The radius of each newly created circle
    static constexpr float kDefaultRadius = 20.0f;

    qreal radius_;

    QColor color_;
};

#endif // CIRCLE_H
