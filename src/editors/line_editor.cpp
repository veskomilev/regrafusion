// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QColorDialog>

#include "editors/line_editor.h"
#include "gfx/leaves/line.h"

LineEditor::LineEditor(QGridLayout *grid, uint grid_row)
{
    type_ = leaf_type_t::line;
    LineEditor::setupWidgets(grid, grid_row);
    hideWidgets();
}

LineEditor::~LineEditor()
{

}

void LineEditor::setupWidgets(QGridLayout *grid, uint grid_row)
{
    QGridLayout *subgrid = new QGridLayout();
    grid->addLayout(subgrid, grid_row, 0, 1, 2, Qt::AlignTop);

    setupSingleValueControl(subgrid, &x1_label_, &x1_editor_, "x1:", 0);
    setupSingleValueControl(subgrid, &y1_label_, &y1_editor_, "y1:", 1);
    setupSingleValueControl(subgrid, &x2_label_, &x2_editor_, "x2:", 2);
    setupSingleValueControl(subgrid, &y2_label_, &y2_editor_, "y2:", 3);
    setupColorControl(subgrid, &color_label_, &color_editor_, "color:", 4);

    connect(x1_editor_, &QLineEdit::textEdited, this, &LineEditor::setConnectedLineProperties);
    connect(x2_editor_, &QLineEdit::textEdited, this, &LineEditor::setConnectedLineProperties);
    connect(y1_editor_, &QLineEdit::textEdited, this, &LineEditor::setConnectedLineProperties);
    connect(y2_editor_, &QLineEdit::textEdited, this, &LineEditor::setConnectedLineProperties);
    connect(color_editor_, &QPushButton::released, this, &LineEditor::setConnectedLineColor);
}

void LineEditor::update()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(connected_leaf_);
    auto geometry = line->getLine();
    x1_editor_->setText(QString::number(geometry.x1()));
    x2_editor_->setText(QString::number(geometry.x2()));
    y1_editor_->setText(QString::number(geometry.y1()));
    y2_editor_->setText(QString::number(geometry.y2()));
    color_editor_->setPalette(colorToPalette(line->getColor()));
}

void LineEditor::setConnectedLineProperties()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(connected_leaf_);
    auto geometry = line->getLine();
    geometry.setLine(
        valueFromLineEdit(x1_editor_, geometry.x1()),
        valueFromLineEdit(y1_editor_, geometry.y1()),
        valueFromLineEdit(x2_editor_, geometry.x2()),
        valueFromLineEdit(y2_editor_, geometry.y2())
        );
    line->setLine(geometry);

    emit propertyEdited();
}

void LineEditor::setConnectedLineColor()
{
    if (!isConnected()) {
        return;
    }

    std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(connected_leaf_);
    QColor color = QColorDialog::getColor(line->getColor());

    if (!color.isValid()) {
        // don't set the color if the user has closed the dialog without choosing one
        return;
    }

    line->setColor(color);
    color_editor_->setPalette(colorToPalette(line->getColor()));

    emit propertyEdited();
}
