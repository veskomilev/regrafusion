// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file transform_editor.h */

#ifndef TRANSFORM_EDITOR_H
#define TRANSFORM_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

//!  Transformation matrix editor for leaves of all types
class TransformEditor : public Editor
{
public:
    //!
    //! \param grid The parent grid that should hold the newly created grid.
    //! \param grid_row The row of the parent grid that should hold the subgrid.
    //!
    TransformEditor(QGridLayout *grid, uint grid_row);

    ~TransformEditor();

    //!
    //! Inherits the functionality of the parent class and also hooks up the connected leaf's transformedNatively() to update(),
    //! so that editor values are updated each time the user moves a leaf with the cursor
    //!
    //! \param leaf The leaf to be controlled
    //! \param leaf_depth The leaf to be controlled's depth
    //!
    void connectToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth);

    //!
    //! Utility function to check if the leaf just selected is the same as the currently selected one
    //!
    bool isItTheSameLeaf(std::shared_ptr<Leaf> leaf) const;

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
    //! Sets the connected leaf's transformations to the values set in the widgets
    //!
    void setConnectedLeafTransformation();

    QLineEdit *x_position_editor_;
    QLineEdit *y_position_editor_;
    QLineEdit *rotation_editor_;
    QLineEdit *x_scale_editor_;
    QLineEdit *y_scale_editor_;

    QLabel *x_position_label_;
    QLabel *y_position_label_;
    QLabel *rotation_label_;
    QLabel *x_scale_label_;
    QLabel *y_scale_label_;
};

#endif // TRANSFORM_EDITOR_H
