// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file displaywidget.h */

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPoint>
#include <QStatusBar>
#include <QOpenGLWidget>

#include "gfx/leaf.h"
#include "view.h"

//!  A helper struct to represent the state of the 'ghost shape' that is displayed during Drag-and-Drop events
struct dragged_leaf_t
{
    bool exists; //!<  Whether the ghost shape is present
    leaf_type_t leaf_type; //!<  Leaf type of the ghost shape
    QPointF position; //!<  Position of the ghost shape
};

class RgfCtx;

//!  DisplayWidget is the UI widget of the view area.

//!  It draws the main (user view) graphical buffer and processes UI events from the user. It is the main point of interaction with the program.
class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    DisplayWidget(QWidget* parent = NULL);
\
    //!
    //! This is the main drawing function. It draws the active buffer to the OpenGL canvas. The active buffer is usually the user view buffer,
    //! but it can be switched to the color id buffer for debug purposes.
    //!
    //! \sa switchBuffers(), draw_user_view_buffer_
    //!
    void paintGL() override;

    //!
    //! Resizes the OpenGL canvas and updates the internal View structure accordingly. Invocation of this function is handled by Qt framework.
    //!
    //! \param w New canvas width
    //! \param h New canvas height
    //! \sa View
    //!
    void resizeGL(int w, int h) override;

    //!
    //! Resets viewport position to its identity, i.e. (0, 0)
    //!
    void resetViewPosition();

    //!
    //! Resets viewport scale to its identity, i.e. 1
    //!
    void resetViewScale();

    //!
    //! Sets the status bar pointer, so that it can be updated with information about viewport position and scale
    //!
    //! \param bar The status bar
    //! \sa View, updateStatus()
    //!
    void setStatusBar(QStatusBar* const& bar);
    // TODO: add RgfCtx public function to set the text instead of this

    //!
    //! Switches the active buffer between user view buffer and color id buffer
    //!
    //! \sa paintGl(), draw_user_view_buffer_
    //!
    void switchBuffers();

    void setRgfCtx(std::shared_ptr<RgfCtx> ctx) { ctx_ = ctx; }

    //!
    //! Updates the status bar with information about viewport position and scale
    //!
    //! \sa View
    //!
    void updateStatus();

    View getView() const { return view_; }

    //!
    //! Captures mouse tracking events and passes them to the event filter. Without this functionality, mouse move events are only processed
    //! when there's dragging going on.
    //!
    //! \param event The event to pass to eventFilter()
    //! \sa eventFilter()
    //!
    void mouseMoveEvent(QMouseEvent *event);

private:
    //!
    //! Initializes draw buffers with background colors and other default setttings
    //!
    //! \param painter Pointer to the painter of the user view buffer
    //! \param color_id_painter Pointer to the painter of the color id buffer
    //!
    void initializeDrawBuffers(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter);

    //!
    //! Main entry point of view area UI event handling. Events are sorted and handled depending on internal state and event type.
    //!
    //! \param obj Watched object (unused inherited argument)
    //! \param event The event to handle
    //! \return True if the event has been processed and should be blocked from being handled by Qt framework
    //!
    bool eventFilter(QObject *obj, QEvent *event) override;

    //!
    //! Translates the viewport by the amount the cursor has moved with respect to the previous update
    //!
    //! \param event The mouse event that contains new cursor position information
    //! \sa previous_mouse_position_
    //!
    void moveView(QMouseEvent *mouseEvent);

    //!
    //! Translates a leaf by the amount the cursor has moved with respect to the previous update
    //!
    //! \param event The mouse event that contains new cursor position information
    //! \aram leaf The leaf to translate
    //! \sa previous_mouse_position_, Leaf
    //!
    void moveLeaf(QMouseEvent *mouseEvent, std::shared_ptr<Leaf> leaf);

    //!
    //! Handles key press events
    //!
    //! \param event The event to handle
    //!
    void keyPressEvent(QKeyEvent *event) override;

    //!
    //! Handles mouse wheel press events
    //!
    //! \param event The event to handle
    //!
    void wheelEvent(QWheelEvent *event) override;

    //!
    //! Limits view position to mitigate artifacts due to loss of floating point precision
    //!
    //! \sa View::kMaxViewOffset
    //!
    void limitViewPosition();

    //!
    //! Processses the drop part of Drag-and-Drop events; more specifically, it creates a new leaf in the place of its ghost image
    //!
    //! \param event The event to process
    //!
    void dropEvent(QDropEvent *event);

    //!
    //! Processes the beginning of a Drag-and-Drop event
    //!
    //! \param event The event to process
    //!
    void dragEnterEvent(QDragEnterEvent *event);

    //!
    //! Processses the drag part of Drag-and-Drop events; more specifically, it updates the position of the ghost image
    //!
    //! \param event The event to process
    //!
    void dragMoveEvent(QDragMoveEvent *event);

    //!
    //! Processes the exiting of a Drag-and-Drop event, i.e. the cursor has left the view area without anything being dropped
    //!
    //! \param event The event to process
    //!
    void dragLeaveEvent(QDragLeaveEvent *event);

    //!
    //! Draws a ghost image of a dragged leaf if a Drag-and-Drop event is occurring
    //!
    //! \param painter A pointer to the user view painter
    //!
    void drawDraggedLeaf(std::shared_ptr<QPainter> painter);

    QStatusBar* status_bar_;

    View view_;

    //!  Position of the cursor during the previous frame, used for the purposes of calculating frame-to-frame translation
    //!  \sa moveView()
    QPointF previous_mouse_position_;

    std::shared_ptr<RgfCtx> ctx_;

    //!  Whether to draw the user view buffer or color id buffer in paintGL()
    //!  \sa paintGL(), moveLeaf()
    bool draw_user_view_buffer_;

    //!  State information about the dragged leaf
    dragged_leaf_t dragged_leaf_;

    //!  Indicator whether mouse dragging is occurring
    bool mouse_dragged_;
};

#endif // DISPLAYWIDGET_H
