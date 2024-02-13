// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "editors/transform_editor.h"
#include "math_utils.h"

TransformEditor::TransformEditor(QGridLayout *grid, uint grid_row)
{
    TransformEditor::setupWidgets(grid, grid_row);
    hideWidgets();
}

TransformEditor::~TransformEditor()
{

}

void TransformEditor::setupWidgets(QGridLayout *grid, uint grid_row)
{
    QGridLayout *subgrid = new QGridLayout();
    grid->addLayout(subgrid, grid_row, 0, 1, 2, Qt::AlignTop);
    setupSingleValueControl(subgrid, &x_position_label_, &x_position_editor_, "x:");
    setupSingleValueControl(subgrid, &y_position_label_, &y_position_editor_, "y:");
    setupSingleValueControl(subgrid, &rotation_label_, &rotation_editor_, "rotation:");
    setupSingleValueControl(subgrid, &x_scale_label_, &x_scale_editor_, "x scale:");
    setupSingleValueControl(subgrid, &y_scale_label_, &y_scale_editor_, "y scale:");

    connect(x_position_editor_, &QLineEdit::textEdited, this, &TransformEditor::setConnectedLeafTransformation);
    connect(y_position_editor_, &QLineEdit::textEdited, this, &TransformEditor::setConnectedLeafTransformation);
    connect(rotation_editor_, &QLineEdit::textEdited, this, &TransformEditor::setConnectedLeafTransformation);
    connect(x_scale_editor_, &QLineEdit::textEdited, this, &TransformEditor::setConnectedLeafTransformation);
    connect(y_scale_editor_, &QLineEdit::textEdited, this, &TransformEditor::setConnectedLeafTransformation);
}

void TransformEditor::connectToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth)
{
    Editor::connectToLeaf(leaf, leaf_depth);

    if (!isConnected()) {
        return;
    }

    connect(connected_leaf_.get(), &Leaf::transformedNatively, this, &TransformEditor::update);
}

bool TransformEditor::isItTheSameLeaf(std::shared_ptr<Leaf> leaf) const
{
    return leaf == connected_leaf_;
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

void TransformEditor::setConnectedLeafTransformation()
{
    if (!isConnected()) {
        return;
    }

    TransformationInfo tfm = decomposeMatrix(connected_leaf_->matrix());

    tfm.location.rx() = valueFromLineEdit(x_position_editor_, tfm.location.rx());
    tfm.location.ry() = valueFromLineEdit(y_position_editor_, tfm.location.ry());
    tfm.rotation_deg = valueFromLineEdit(rotation_editor_, tfm.rotation_deg);
    tfm.scale.rx() = valueFromLineEdit(x_scale_editor_, tfm.scale.rx());
    tfm.scale.ry() = valueFromLineEdit(y_scale_editor_, tfm.scale.ry());

    connected_leaf_->setTransformationMatrix(
        QTransform().
        translate(tfm.location.rx(), tfm.location.ry()).
        scale(tfm.scale.rx(), tfm.scale.ry()).
        rotate(tfm.rotation_deg)
        );

    emit propertyEdited();
}
