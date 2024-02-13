// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QColorDialog>

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

    setupSingleValueControl(subgrid, &radius_label_, &radius_editor_, "radius:");
    setupColorControl(subgrid, &color_label_, &color_editor_, "color:");

    connect(radius_editor_, &QLineEdit::textEdited, this, &CircleEditor::setConnectedCircleProperties);
    connect(color_editor_, &QPushButton::released, this, &CircleEditor::setConnectedCircleColor);
}

void CircleEditor::update()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(connected_leaf_);
    radius_editor_->setText(QString::number(circle->getRadius()));
    color_editor_->setPalette(colorToPalette(circle->getColor()));
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

void CircleEditor::setConnectedCircleColor()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(connected_leaf_);
    QColor color = QColorDialog::getColor(circle->getColor());

    if (!color.isValid()) {
        // don't set the color if the user has closed the dialog without choosing one
        return;
    }

    circle->setColor(color);
    color_editor_->setPalette(colorToPalette(circle->getColor()));

    emit propertyEdited();
}
