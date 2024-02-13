// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QColorDialog>

#include "editors/rectangle_editor.h"
#include "gfx/leaves/rectangle.h"

RectangleEditor::RectangleEditor(QGridLayout *grid, uint grid_row)
{
    type_ = leaf_type_t::rectangle;
    RectangleEditor::setupWidgets(grid, grid_row);
    hideWidgets();
}

RectangleEditor::~RectangleEditor()
{

}

void RectangleEditor::setupWidgets(QGridLayout *grid, uint grid_row)
{
    QGridLayout *subgrid = new QGridLayout();
    grid->addLayout(subgrid, grid_row, 0, 1, 2, Qt::AlignTop);

    setupSingleValueControl(subgrid, &x1_label_, &x1_editor_, "x1:");
    setupSingleValueControl(subgrid, &y1_label_, &y1_editor_, "y1:");
    setupSingleValueControl(subgrid, &x2_label_, &x2_editor_, "x2:");
    setupSingleValueControl(subgrid, &y2_label_, &y2_editor_, "y2:");
    setupColorControl(subgrid, &color_label_, &color_editor_, "color:");

    connect(x1_editor_, &QLineEdit::textEdited, this, &RectangleEditor::setConnectedRectangleProperties);
    connect(x2_editor_, &QLineEdit::textEdited, this, &RectangleEditor::setConnectedRectangleProperties);
    connect(y1_editor_, &QLineEdit::textEdited, this, &RectangleEditor::setConnectedRectangleProperties);
    connect(y2_editor_, &QLineEdit::textEdited, this, &RectangleEditor::setConnectedRectangleProperties);
    connect(color_editor_, &QPushButton::released, this, &RectangleEditor::setConnectedRectangleColor);
}

void RectangleEditor::update()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Rectangle> rectangle = std::dynamic_pointer_cast<Rectangle>(connected_leaf_);
    auto geometry = rectangle->getRectangle();
    x1_editor_->setText(QString::number(geometry.left()));
    x2_editor_->setText(QString::number(geometry.right()));
    y1_editor_->setText(QString::number(geometry.top()));
    y2_editor_->setText(QString::number(geometry.bottom()));
    color_editor_->setPalette(colorToPalette(rectangle->getColor()));
}

void RectangleEditor::setConnectedRectangleProperties()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Rectangle> rectangle = std::dynamic_pointer_cast<Rectangle>(connected_leaf_);
    auto geometry = rectangle->getRectangle();
    geometry.setCoords(
        valueFromLineEdit(x1_editor_, geometry.left()),
        valueFromLineEdit(y1_editor_, geometry.top()),
        valueFromLineEdit(x2_editor_, geometry.right()),
        valueFromLineEdit(y2_editor_, geometry.bottom())
        );
    rectangle->setRectangle(geometry);

    emit propertyEdited();
}

void RectangleEditor::setConnectedRectangleColor()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Rectangle> rectangle = std::dynamic_pointer_cast<Rectangle>(connected_leaf_);
    QColor color = QColorDialog::getColor(rectangle->getColor());

    if (!color.isValid()) {
        // don't set the color if the user has closed the dialog without choosing one
        return;
    }

    rectangle->setColor(color);
    color_editor_->setPalette(colorToPalette(rectangle->getColor()));

    emit propertyEdited();
}
