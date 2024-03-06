// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file circle_editor.h */

#ifndef CIRCLE_EDITOR_H
#define CIRCLE_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

//!  Editor for leaves of circle type
class CircleEditor : public Editor
{
public:
    //!
    //! \param grid The parent grid that should hold the newly created grid.
    //! \param grid_row The row of the parent grid that should hold the subgrid.
    //!
    CircleEditor(QGridLayout *grid, uint grid_row);

    ~CircleEditor();

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
    //! Sets the numerical (non color) properties of the connected leaf of circle type to the values set in the widgets
    //!
    void setConnectedCircleProperties();

    //!
    //! Sets the color of the connected leaf of circle type to the value set in the color input widget
    //!
    void setConnectedCircleColor();

    QLineEdit *radius_editor_;
    QPushButton *color_editor_;

    QLabel *radius_label_;
    QLabel *color_label_;
};

#endif // CIRCLE_EDITOR_H
