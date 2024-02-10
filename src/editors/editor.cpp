// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "editors/editor.h"

Editor::Editor() {

}

Editor::~Editor() {

}

void Editor::setupSingleValueControl(QGridLayout *grid, QLabel **label, QLineEdit **line_edit, QString label_text, uint row)
{
    *label = new QLabel();
    (*label)->setText(label_text);

    *line_edit = new QLineEdit();
    (*line_edit)->setValidator(&double_validator_);
    (*line_edit)->setText("0");

    grid->addWidget(*label, row, 0, 1, 1, Qt::AlignTop);
    grid->addWidget(*line_edit, row, 1, 1, 1, Qt::AlignTop);

    widgets_.push_back(*label);
    widgets_.push_back(*line_edit);
}
