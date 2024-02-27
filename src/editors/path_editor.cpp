// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QColorDialog>

#include "editors/path_editor.h"
#include "gfx/leaves/path.h"

PathEditor::PathEditor(QGridLayout *grid, uint grid_row)
{
    type_ = leaf_type_t::path;
    PathEditor::setupWidgets(grid, grid_row);
    hideWidgets();
}

PathEditor::~PathEditor()
{

}

void PathEditor::setupWidgets(QGridLayout *grid, uint grid_row)
{
    QGridLayout *subgrid = new QGridLayout();
    grid->addLayout(subgrid, grid_row, 0, 1, 2, Qt::AlignTop);

    setupColorControl(subgrid, &color_label_, &color_editor_, "color:");

    connect(color_editor_, &QPushButton::released, this, &PathEditor::setConnectedPathColor);
}

void PathEditor::update()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Path> path = std::dynamic_pointer_cast<Path>(connected_leaf_);
    color_editor_->setPalette(colorToPalette(path->getColor()));
}

void PathEditor::setConnectedPathColor()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Path> path = std::dynamic_pointer_cast<Path>(connected_leaf_);
    QColor color = QColorDialog::getColor(path->getColor());

    if (!color.isValid()) {
        // don't set the color if the user has closed the dialog without choosing one
        return;
    }

    path->setColor(color);
    color_editor_->setPalette(colorToPalette(path->getColor()));

    emit propertyEdited();
}
