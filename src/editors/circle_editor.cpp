// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "editors/circle_editor.h"
#include "gfx/leaves/circle.h"

CircleEditor::CircleEditor(QGridLayout *grid, uint grid_row)
{
    type_ = leaf_type_t::circle;
    CircleEditor::setupWidgets(grid, grid_row);
    CircleEditor::hideWidgets();
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

void CircleEditor::showWidgets()
{
    for (auto &widget : widgets_) {
        widget->show();
    }
}

void CircleEditor::hideWidgets()
{
    for (auto &widget : widgets_) {
        widget->hide();
    }

}

void CircleEditor::connectToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth)
{
    if (leaf == nullptr || leaf == connected_leaf_) {
        return;
    }

    if (leaf->getType() != leaf_type_t::circle) {
        return;
    }

    if (isConnected()) {
        disconnectFromLeaf();
    }

    // change state here
    state_ = state_t::connected;

    connected_leaf_ = leaf;
    connected_leaf_depth_ = leaf_depth;

    // TODO: connect to circle here if widget editing is implemented
    update();
    showWidgets();
}

void CircleEditor::disconnectFromLeaf()
{
    if (!isConnected()) {
        return;
    }

    // change state here
    state_ = state_t::disconnected;

    disconnect(connected_leaf_.get(), nullptr, this, nullptr);
    connected_leaf_ = nullptr;
    connected_leaf_depth_ = 0;
    hideWidgets();
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
