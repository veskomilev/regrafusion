// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file viewer.h
    \brief Contains definition of Viewer class and it's also an entry point from Qt.
*/

#ifndef VIEWER_H
#define VIEWER_H

#include <memory>
#include <QMainWindow>
#include <string>
#include <vector>

#include "rgf_ctx.h"

#include "editors/editor.h"
#include "editors/circle_editor.h"
#include "editors/line_editor.h"
#include "editors/path_editor.h"
#include "editors/rectangle_editor.h"
#include "editors/transform_editor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class viewer;
}
QT_END_NAMESPACE

//!  Viewer is the class of the program window. It holds points to main program widgets, as well as the program context (RgfCtx).
class viewer : public QMainWindow
{
    Q_OBJECT

public:
    //!
    //! The constructor initializes major UI elements, as well as the program context (RgfCtx)
    //!
    //! \sa RgfCtx
    //!
    viewer(QWidget *parent = nullptr);

    ~viewer();

private slots:
    void on_reset_view_button_pressed();

    void on_reset_scale_button_pressed();

    void on_num_branches_slider_valueChanged(int value);

    void on_num_branches_spin_box_valueChanged(int arg1);

    void on_switch_buffers_pressed();

    //!
    //! Enables or disables edit mode QActions depending on context mode
    //!
    void onRgfCtxModeSwitched();

    //!
    //! Notifies all applicable editors to connect to the selected leaf.
    //!
    //! \param leaf Pointer to the selected leaf
    //! \param leaf_depth Depth of the selected leaf instance
    //!
    void onLeafSelected(std::shared_ptr<Leaf> leaf, uint leaf_depth);

private:
    void setupToolbar();

    void setupEditors();

    //!
    //! Utility function to obtain the next free row in the main grid layout to the right
    //!
    //! \return The next free row
    //!
    int getNextFreeRowInGridLayout();

    void enableEditModeActions();

    void disableEditModeActions();

    //!
    //! Adds a new edit mode action (ie widget)
    //!
    //! \param toolbar Toolbar which should contain the new action
    //! \param resource_path Path to the icon to be used for the button
    //! \param description Description of the action
    //! \param status_tip Tooltip to be used for the button
    //! \param type The type of the leaf that should be added when the action is invoked
    //!
    void addEditModeAction(QToolBar* toolbar, std::string resource_path, std::string description, std::string status_tip, leaf_type_t type);

    //!  Qt framework internal pointer
    Ui::viewer *ui;

    std::shared_ptr<RgfCtx> ctx_;

    std::vector<QAction *> edit_mode_actions_;

    std::shared_ptr<TransformEditor> tfm_editor_;

    std::shared_ptr<CircleEditor> circle_editor_;

    std::shared_ptr<LineEditor> line_editor_;

    std::shared_ptr<RectangleEditor> rectangle_editor_;

    std::shared_ptr<PathEditor> path_editor_;

    std::vector<std::shared_ptr<Editor>> editors_;

};
#endif // VIEWER_H
