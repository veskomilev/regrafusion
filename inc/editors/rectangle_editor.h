// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file rectangle_editor.h */

#ifndef RECTANGLE_EDITOR_H
#define RECTANGLE_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

//!  Editor for leaves of rectangle type
class RectangleEditor : public Editor
{
public:
    //!
    //! \param grid The parent grid that should hold the newly created grid.
    //! \param grid_row The row of the parent grid that should hold the subgrid.
    //!
    RectangleEditor(QGridLayout *grid, uint grid_row);

    ~RectangleEditor();

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
    //! Sets the numerical (non color) properties of the connected leaf of rectangle type to the values set in the widgets
    //!
    void setConnectedRectangleProperties();

    //!
    //! Sets the color of the connected leaf of rectangle type to the value set in the color input widget
    //!
    void setConnectedRectangleColor();

    QLineEdit *x1_editor_;
    QLineEdit *x2_editor_;
    QLineEdit *y1_editor_;
    QLineEdit *y2_editor_;
    QPushButton *color_editor_;

    QLabel *x1_label_;
    QLabel *x2_label_;
    QLabel *y1_label_;
    QLabel *y2_label_;
    QLabel *color_label_;
};

#endif // RECTANGLE_EDITOR_H
