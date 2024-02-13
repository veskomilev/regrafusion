// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "editors/circle_editor.h"
#include "gfx/leaves/circle.h"

CircleEditor::CircleEditor(QGridLayout *grid, uint grid_row)
{
    type_ = leaf_type_t::circle;
    CircleEditor::setupWidgets(grid, grid_row);
    hideWidgets();
}

CircleEditor::~CircleEditor()
{

}

void CircleEditor::setupWidgets(QGridLayout *grid, uint grid_row)
{
    QGridLayout *subgrid = new QGridLayout();
    grid->addLayout(subgrid, grid_row, 0, 1, 2, Qt::AlignTop);

    setupSingleValueControl(subgrid, &radius_label_, &radius_editor_, "radius:", 0);
    connect(radius_editor_, &QLineEdit::textEdited, this, &CircleEditor::setConnectedCircleProperties);
}

void CircleEditor::update()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(connected_leaf_);
    radius_editor_->setText(QString::number(circle->getRadius()));
}

void CircleEditor::setConnectedCircleProperties()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(connected_leaf_);
    circle->setRadius(valueFromLineEdit(radius_editor_, circle->getRadius()));

    emit propertyEdited();
}
