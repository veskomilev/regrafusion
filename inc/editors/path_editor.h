// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file path_editor.h */

#ifndef PATH_EDITOR_H
#define PATH_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

//!  Editor for leaves of path type
class PathEditor : public Editor
{
public:
    //!
    //! \param grid The parent grid that should hold the newly created grid.
    //! \param grid_row The row of the parent grid that should hold the subgrid.
    //!
    PathEditor(QGridLayout *grid, uint grid_row);

    ~PathEditor();

private:
    //!
    //! Creates the widgets specific to the Editor. All editor widgets are contained within their own, newly created grid.
    //!
    //! \param grid The parent grid that should hold the newly created grid.
    //! \param grid_row The row of the parent grid that should hold the subgrid.
    //!
    void setupWidgets(QGridLayout *grid, uint grid_row);

    //!
    //! Updates all editors' values with current data from the controlled leaf
    //!
    void update();

    //!
    //! Sets the color of the connected leaf of path type to the value set in the color input widget
    //!
    void setConnectedPathColor();

    QPushButton *color_editor_;

    QLabel *color_label_;
};

#endif // PATH_EDITOR_H
