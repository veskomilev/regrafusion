// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "editors/editor.h"

Editor::Editor() :
    connected_leaf_(nullptr),
    connected_leaf_depth_(0),
    state_(state_t::disconnected)
{
}

Editor::~Editor()
{
}

void Editor::setupSingleValueControl(QGridLayout *grid, QLabel **label, QLineEdit **line_edit, QString label_text, uint row)
{
    *label = new QLabel();
    (*label)->setText(label_text);

    *line_edit = new QLineEdit();
    (*line_edit)->setText("0");

    grid->addWidget(*label, row, 0, 1, 1, Qt::AlignTop);
    grid->addWidget(*line_edit, row, 1, 1, 1, Qt::AlignTop);

    widgets_.push_back(*label);
    widgets_.push_back(*line_edit);
}

double Editor::valueFromLineEdit(QLineEdit *line_editor, double fallback)
{
    bool success = false;
    double value = line_editor->text().toDouble(&success);

    if (success) {
        return value;
    } else {
        return fallback;
    }
}
