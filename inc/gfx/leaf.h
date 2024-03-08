// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file leaf.h */

#ifndef LEAF_H
#define LEAF_H

#include <QColor>
#include <QEvent>
#include <QMimeData>
#include <QPainter>
#include <QTransform>

#include "common.h"
#include "controls/control.h"

class RgfCtx;

//!  Leaf is the abstract parent class of all objects (shapes) displayed in the view area, as well as of the spawn points
class Leaf : public QObject
{
    Q_OBJECT
protected:
    //!
    //! New leaves are meant to be constructed only by calling the factory method constructNew().
    //!
    //! \param ctx A pointer to the context
    //! \param type Which type the newly created leaf is
    //! \sa constructNew()
    //!
    Leaf(std::weak_ptr<RgfCtx> ctx, leaf_type_t type);

public:
    virtual ~Leaf();

    //!
    //! Extracts leaf type from QMimeData, which is generated by Drag-and-Drop events
    //!
    //! \param mime_data The data that contains leaf type
    //! \return The extracted leaf type
    //!
    static leaf_type_t extractType(const QMimeData *mime_data);

    //!
    //! Draws a leaf instance onto the view area and the color id buffer
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //! \param color_id_painter A pointer to painter that paints onto the color id buffer
    //! \param depth Which consecutive branch the leaf is on
    //!
    virtual void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth);

    //!
    //! Draws a ghost shape under the cursor when a Drag-and-Drop event is occurring to visualise where exactly the new leaf would be
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //! \param leaf_type Which type the newly created leaf would be
    //! \param position Where to draw the ghost shape
    //! \param scale The scale of the ghost shape
    //!
    static void drawDragged(std::shared_ptr<QPainter> painter, leaf_type_t leaf_type, QPointF position, qreal scale);

    //!
    //! Factory method for instantiating new leaves
    //!
    //! \param ctx A pointer to the context
    //! \param leaf_type Which type the newly created leaf should be
    //! \return A pointer to the newly created leaf
    //!
    static std::shared_ptr<Leaf> constructNew(std::shared_ptr<RgfCtx> ctx, leaf_type_t leaf_type);

    //!
    //! \return Whether the leaf is a spawn point
    //!
    virtual inline bool isSpawnPoint() = 0;

    //!
    //! A setter function for the transformation matrix. It succeeds only if the parameter is an invertible matrix,
    //! i.e. if it's of non zero scale and if it doesn't squish the shape onto a single line.
    //! On failure the leaf's current matrix remains unchanged.
    //!
    //! \param matrix The new would-be matrix
    //! \return Whether the setting was successful
    //!
    bool setTransformationMatrix(QTransform matrix);

    QTransform& matrix() { return matrix_; }

    //!
    //! A utility function to remove the effects of the leaf's matrix from a painter's transformations
    //!
    //! \param painter A pointer to the painter that should "forget" this leaf's transformations
    //!
    inline void unapplyLocalTransformations(std::shared_ptr<QPainter> painter) {
        if (!matrix_.isIdentity()) {
            // combine param is true, so combine matrix_ with the existing transformations instead of overwriting them
            painter->setWorldTransform(matrix_.inverted(), true);
        }
    }

    QColor getColorId() const { return color_id_; }

    //!
    //! Sets internal selected_ flag to true and creates the associated leaf controls
    //!
    virtual void select();

    //!
    //! Sets internal selected_ flag to false and destroys the associated leaf controls
    //!
    void deselect();

    //!
    //! Passes UI events to controls, in case they should affect this leaf. This function is called only if the leaf is selected.
    //! If an event has been used for control, it is blocked so that it isn't handled twice (e.g. so that a mouse move event doesn't
    //! both move a leaf and move the view).
    //!
    //! \param event The event to be processed
    //! \return True if the event has been processed and should be blocked
    //!
    bool passthroughEvent(QEvent *event);

    //!
    //! Maps a coordinate from screen space to this leaf's local space
    //!
    //! \param ctx A pointer to the context
    //! \param coordinate The coordinate to move mapped
    //! \return The mapped coordinate
    //!
    QPointF fromSreenSpace(std::shared_ptr<RgfCtx> ctx, QPointF coordinate);

    //!
    //! A setter for the matrix when the leaf is dragged by the cursor. It emits transformedNatively(), so that editors can be updated
    //!
    //! \param translation The amount the matrix should be translated by
    //!
    void translateNatively(QPointF translation);

    leaf_type_t getType() const { return type_; }

    //!
    //! Draws all controls onto the view area
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //!
    void drawControls(std::shared_ptr<QPainter> painter);

    bool isSelected() const { return selected_; }

signals:
    //!
    //! Signal used to notify editors that they should update their widgets with the leaf's current matrix
    //!
    void transformedNatively();

protected:
    //!
    //! Calculates the color using which the leaf instance should be drawn in the color id buffer, based on its color id and its depth
    //!
    //! \param depth The depth this leaf instance is at
    //! \return True if the event has been processed and should be blocked
    //!
    QColor getUniqueColor(uint depth);

    //!
    //! Creates controls (widgets in the view area) to modify the leaf
    //!
    virtual void createControls() {}

    //!
    //! Destroys any created controls
    //!
    void destroyControls();

    std::weak_ptr<RgfCtx> ctx_;

    //!  A color that uniquely identifies the leaf in the color id buffer (though this isn't necessarily the color used to draw
    //! the leaf there - see getUniqueColor())
    QColor color_id_;

    //!  Whether the leaf is selected in the view area
    bool selected_;

    //!  A vector of controls that modify the leaf
    std::vector<std::shared_ptr<Control>> controls_;

    //!  A reference to self that is passed to any created control
    std::weak_ptr<Leaf> self_ref_;

private:
    // disable copy and assignment ctors
    Leaf(const Leaf&) = delete;
    Leaf& operator=(const Leaf&) = delete;

    //!  This leaf's type
    leaf_type_t type_;

    //!  This leaf's transformation matrix
    QTransform matrix_;
};


#endif // LEAF_H
