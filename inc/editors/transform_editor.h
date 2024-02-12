// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef TRANSFORM_EDITOR_H
#define TRANSFORM_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

class TransformEditor : public Editor
{
public:
    TransformEditor(QGridLayout *grid);

    ~TransformEditor();

    void setupWidgets(QGridLayout *grid);

    void showWidgets(std::shared_ptr<Leaf> leaf, uint leaf_depth);

    void hideWidgets();

private:
    void attachToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth);

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