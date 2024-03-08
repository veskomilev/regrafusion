// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file editor.h */

#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include <vector>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QPalette>
#include <QPushButton>
#include <QWidget>

#include "common.h"
#include "gfx/leaf.h"

#include <QLabel>
#include <QLineEdit>

//! An abstract parent class for "editors" (i.e. widgets in the UI next to the view area) that are used to edit individual leaves.

//! Editors consist of UI elements such as text inputs and color selectors. Editors happen to be singletons, i.e. they are created once at
//! program start and then are activated and deactived (i.e. connected to a leaf and shown, resp. disconnected and hidden) when necessary.
//!
class Editor : public QObject
{
    Q_OBJECT
public:
    Editor();

    virtual ~Editor();

    //!
    //! Used by Viewer class to make a Leaf controllable by the Editor and to synchronize data between the two instances
    //!
    //! \param leaf The leaf to be controlled
    //! \param leaf_depth The leaf instance's depth, on top of which the editor should be drawn
    //!
    void connectToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth);

    //!
    //! Used by Viewer class to make no Leaf controllable by the Editor
    //!
    void disconnectFromLeaf();

    enum class state_t {
        disconnected = 0,
        connected
    };

    bool isConnected() const { return state_ == state_t::connected; }

    leaf_type_t getType() const { return type_; }

signals:
    //!
    //! Emitted when an input widget's value, resp. controlled proprety has been modified. It's used to refresh the view area.
    //!
    void propertyEdited();

protected:
    //!
    //! Creates the widgets specific to the Editor. All editor widgets are contained within their own, newly created grid.
    //!
    //! \param grid The parent grid that should hold the newly created grid.
    //! \param grid_row The row of the parent grid that should hold the subgrid.
    //!
    virtual void setupWidgets(QGridLayout *grid, uint grid_row) = 0;

    //!
    //! Creates a text editor to control a single property.
    //!
    //! \param grid The grid that should contain the text editor
    //! \param label A nullptr to QLabel that is created and contains description of the property
    //! \param line_edit A nullptr to QLineEdit that created and set up to be the property editor
    //! \param label_text The property's description
    //!
    void setupSingleValueControl(QGridLayout *grid, QLabel **label, QLineEdit **line_edit, QString label_text);

    //!
    //! Creates a color editor to control a single color property.
    //!
    //! \param grid The grid that should contain the text editor
    //! \param label A nullptr to QLabel that is created and contains description of the property
    //! \param push_button A nullptr to QPushButton that created and set up to be the property editor
    //! \param label_text The property's description
    //!
    void setupColorControl(QGridLayout *grid, QLabel **label, QPushButton **push_button, QString label_text);

    //!
    //! Obtains a numerical value from a text editor
    //!
    //! \param line_editor The widget to get the value from
    //! \param fallback The fallback value that is returned in case the widget doesn't contain a valid numerical value
    //! \return The obtained numerical value
    //!
    double valueFromLineEdit(QLineEdit *line_editor, double fallback);

    //!
    //! Shows all editor's widget, e.g. when a leaf is connected to the editor
    //!
    void showWidgets();

    //!
    //! Hides all editor's widget, e.g. when a leaf is disconnected from the editor
    //!
    void hideWidgets();

    //!
    //! Conversion function to make a color usable as a button's background color for color control widgets
    //!
    QPalette colorToPalette(QColor color);

    //!
    //! Updates all editors' values with current data from the controlled leaf
    //!
    virtual void update() = 0;

    //!  A vector that contains all editors' widgets
    std::vector<QWidget *> widgets_;

    //!  A pointer to the currently connected leaf
    std::shared_ptr<Leaf> connected_leaf_;

    //!  The connected leaf instance's depth, on top of which the editor is drawn
    uint connected_leaf_depth_;

    //!  A value indicating if the editor is connected to a leaf
    state_t state_;

    //!  Connected leaf's type
    leaf_type_t type_;

    //!  The next free row within the editor's own grid layout
    uint next_free_row_;
};

#endif // EDITOR_H
