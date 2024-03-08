// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file path_control.h */

#ifndef PATH_CONTROL_H
#define PATH_CONTROL_H

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "common.h"
#include "controls/control.h"
#include "gfx/leaves/path.h"

//!  A util struct used to return the result of findClosestSideToCursor()
//! \sa findClosestSideToCursor()
typedef struct {
    QPointF a; //!<  The first point's coordinates in screen space
    QPointF b; //!<  The second point's coordinates in screen space
    uint ind; //!<  Index of the first point within the Points vector that holds it
} side_t;

//!  A control for leaves of path type
class PathControl : public Control
{
public:
    //!
    //! \param ctx A pointer to the context
    //! \param leaf The leaf, which will be edited
    //! \param leaf_depth The leaf instance's depth within its tree
    //!
    PathControl(std::shared_ptr<RgfCtx> ctx, std::shared_ptr<Path> leaf, uint leaf_depth);

    ~PathControl();

    //!
    //! Draws the control onto the view area
    //!
    //! \param painter A pointer to the user view painter
    //!
    void draw(std::shared_ptr<QPainter> painter);

    //!
    //! Processes UI events to control the class instance (if applicable)
    //! If an event has been used for the control, it is blocked so that it isn't handled twice (e.g. so that a mouse move event doesn't
    //! both move a leaf and move the view).
    //!
    //! \param event The event to be processed
    //! \return True if the event has been processed and should be blocked
    //!
    bool handleEvent(QEvent *event);

private:
    std::vector<QPointF>& points() { return path_->points(); }

    //!
    //! Draws the control onto the view area when the control is in Move Vertex mode
    //!
    //! \param painter A pointer to the user view painter
    //!
    void drawMoveVertexMode(std::shared_ptr<QPainter> painter);

    //!
    //! Draws the control onto the view area when the control is in Add Vertex mode
    //!
    //! \param painter A pointer to the user view painter
    //!
    void drawAddVertexMode(std::shared_ptr<QPainter> painter);

    //!
    //! Draws the control onto the view area when the control is in Remove Vertex mode
    //!
    //! \param painter A pointer to the user view painter
    //!
    void drawRemoveVertexMode(std::shared_ptr<QPainter> painter);

    //!
    //! Find the side of the controlled path that is currently closest to the cursor (in screen space)
    //!
    //! \return The closest side
    //! \sa side_t
    //!
    side_t findClosestSideToCursor();

    //!
    //! Processes a mouse button press event only if it concerns the control
    //!
    //! \param event The event to be processed
    //! \return True if the event has been processed and should be blocked
    //!
    bool handleMouseButtonPress(QMouseEvent *event);

    //!
    //! Attempts to start dragging a vertex of the selected path near the cursor
    //!
    //! \return True if dragging has started
    //!
    bool startDraggingVertex();

    //!
    //! Adds a vertex
    //!
    //! \return True
    //!
    bool addVertex(); // TODO: change to void

    //!
    //! Attempts to remove a vertex of the selected path near the cursor
    //!
    //! \return True if a vertex has been removed
    //!
    bool removeVertex();

    //!
    //! Processes a mouse move event only if it concerns the control
    //!
    //! \param event The event to be processed
    //! \return True if the event has been processed and should be blocked
    //!
    bool handleMouseMove(QMouseEvent *event);

    //!
    //! Processes a mouse button release event only if it concerns the control
    //!
    //! \param event The event to be processed
    //! \return True if the event has been processed and should be blocked
    //!
    bool handleMouseButtonRelease(QMouseEvent *event);

    //!
    //! Processes a keyboard key press event only if it concerns the control
    //!
    //! \param event The event to be processed
    //!
    void handleKeyPressed(QKeyEvent *event);

    //!
    //! Processes a keyboard key release event only if it concerns the control
    //!
    //! \param event The event to be processed
    //!
    void handleKeyReleased(QKeyEvent *event);

    //!  Current cursor position in screen space
    QPointF mouse_position_;

    //!  Previous cursor position in screen space
    QPointF previous_mouse_position_;

    //!  The radius of tooltip vertix circles that a drawn when the cursor is close enough (in screen space)
    qreal kRadius = 5.0;

    //!  The maximum distance in which the cursor is able to interact with a vertex (dragging, removing, popup tooltips) (in screen space)
    qreal kPopUpDistance = kRadius * 2;

    //!  Indicates if a vertex is currently dragged
    bool vertex_dragged_;

    //!  If a vertex is currently being dragged, this is its index within the Points vector that holds it
    uint dragged_vertex_index_;

    //!  Indicates if the control is currently in Add Vertex mode
    bool add_vertex_mode_;

    //!  Indicates if the control is currently in Remove Vertex mode
    bool remove_vertex_mode_;
    // TODO: add a struct with PathControl modes

    //!  A pointer to the controlled path
    std::shared_ptr<Path> path_;
};

#endif // PATH_CONTROL_H
