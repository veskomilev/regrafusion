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

void TransformEditor::showWidgets(std::shared_ptr<Leaf> leaf, uint leaf_depth)
{
    for (auto &widget : widgets_) {
        widget->show();
    }

    attachToLeaf(leaf, leaf_depth);
}

void TransformEditor::hideWidgets()
{
    for (auto &widget : widgets_) {
        widget->hide();
    }

}

void TransformEditor::attachToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth)
{
    TransformationInfo tfm = decomposeMatrix(leaf->matrix());

    x_position_editor_->setText(QString::number(tfm.location.rx()));
    y_position_editor_->setText(QString::number(tfm.location.ry()));
    rotation_editor_->setText(QString::number(tfm.rotation_deg));
    x_scale_editor_->setText(QString::number(tfm.scale.rx()));
    y_scale_editor_->setText(QString::number(tfm.scale.ry()));
}
