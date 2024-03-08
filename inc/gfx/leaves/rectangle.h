// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file rectangle.h */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "gfx/leaf.h"

//!  A Leaf that is a rectangle
class Rectangle : public Leaf
{
private:
    //!
    //! New leaves are meant to be constructed only by calling the factory method constructNew().
    //!
    //! \param ctx A pointer to the context
    //! \param rectangle The geometry of the rectangle
    //! \param color The color of the rectangle
    //! \sa constructNew()
    //!
    Rectangle(std::weak_ptr<RgfCtx> ctx, QRectF rectangle, QColor color);

public:
    ~Rectangle();

    //!
    //! Factory method for instantiating new leaves
    //!
    //! \param ctx A pointer to the context
    //! \return A pointer to the newly created leaf
    //!
    static std::shared_ptr<Rectangle> constructNew(std::weak_ptr<RgfCtx> ctx);

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

    QRectF getRectangle() const { return rectangle_; }

    void setRectangle(QRectF rectangle) { rectangle_ = rectangle; }

    QColor getColor() const { return color_; }

    void setColor(QColor color) { color_ = color; }

private:
    //!
    //! Creates controls (widgets in the view area) to modify the leaf
    //!
    void createControls() override;

    //!  The geometry of each newly created rectangle
    static constexpr QRectF kDefaultRectangle = QRectF(-10.0f, -10.0f, 20.0f, 20.0f);

    QRectF rectangle_;

    QColor color_;
};

#endif // RECTANGLE_H
