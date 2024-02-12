// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "editors/transform_editor.h"
#include "math_utils.h"

TransformEditor::TransformEditor(QGridLayout *grid)
{
    TransformEditor::setupWidgets(grid);
    TransformEditor::hideWidgets();
}

TransformEditor::~TransformEditor()
{

}

void TransformEditor::setupWidgets(QGridLayout *grid)
{
    setupSingleValueControl(grid, &x_position_label_, &x_position_editor_, "x:", 5);
    setupSingleValueControl(grid, &y_position_label_, &y_position_editor_, "y:", 6);
    setupSingleValueControl(grid, &rotation_label_, &rotation_editor_, "rotation:", 7);
    setupSingleValueControl(grid, &x_scale_label_, &x_scale_editor_, "x scale:", 8);
    setupSingleValueControl(grid, &y_scale_label_, &y_scale_editor_, "y scale:", 9);
}

void TransformEditor::showWidgets()
{
    for (auto &widget : widgets_) {
        widget->show();
    }
}

void TransformEditor::hideWidgets()
{
    for (auto &widget : widgets_) {
        widget->hide();
    }

}

void TransformEditor::connectToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth)
{
    if (leaf == nullptr || leaf == connected_leaf_) {
        return;
    }

    if (isConnected()) {
        disconnectFromLeaf();
    }

    // change state here
    state_ = state_t::connected;

    connected_leaf_ = leaf;
    connected_leaf_depth_ = leaf_depth;

    connect(connected_leaf_.get(), &Leaf::transformedNatively, this, &TransformEditor::update);
    update();
    showWidgets();
}

void TransformEditor::disconnectFromLeaf()
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

void TransformEditor::update()
{
    if (!isConnected()) {
        return;
    }

    TransformationInfo tfm = decomposeMatrix(connected_leaf_->matrix());

    x_position_editor_->setText(QString::number(tfm.location.rx()));
    y_position_editor_->setText(QString::number(tfm.location.ry()));
    rotation_editor_->setText(QString::number(tfm.rotation_deg));
    x_scale_editor_->setText(QString::number(tfm.scale.rx()));
    y_scale_editor_->setText(QString::number(tfm.scale.ry()));
}
