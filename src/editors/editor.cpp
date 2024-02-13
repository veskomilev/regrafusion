// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "editors/editor.h"

Editor::Editor() :
    connected_leaf_(nullptr),
    connected_leaf_depth_(0),
    state_(state_t::disconnected),
    type_(leaf_type_t::invalid),
    next_free_row_(0)
{
}

Editor::~Editor()
{
}

void Editor::connectToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth)
{
    if (leaf == nullptr || leaf == connected_leaf_) {
        return;
    }

    if (leaf->getType() != getType() && getType() != leaf_type_t::invalid) {
        return;
    }

    if (isConnected()) {
        disconnectFromLeaf();
    }

    // change state here
    state_ = state_t::connected;

    connected_leaf_ = leaf;
    connected_leaf_depth_ = leaf_depth;

    update();
    showWidgets();
}

void Editor::disconnectFromLeaf()
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


void Editor::setupSingleValueControl(QGridLayout *grid, QLabel **label, QLineEdit **line_edit, QString label_text)
{
    *label = new QLabel();
    (*label)->setText(label_text);

    *line_edit = new QLineEdit();
    (*line_edit)->setText("0");

    grid->addWidget(*label, next_free_row_, 0, 1, 1, Qt::AlignTop);
    grid->addWidget(*line_edit, next_free_row_, 1, 1, 1, Qt::AlignTop);

    widgets_.push_back(*label);
    widgets_.push_back(*line_edit);

    next_free_row_++;
}

void Editor::setupColorControl(QGridLayout *grid, QLabel **label, QPushButton **push_button, QString label_text)
{
    *label = new QLabel();
    (*label)->setText(label_text);

    *push_button = new QPushButton();
    // make the button look like a color picker
    (*push_button)->setFlat(true);
    (*push_button)->setAutoFillBackground(true);

    grid->addWidget(*label, next_free_row_, 0, 1, 1, Qt::AlignTop);
    grid->addWidget(*push_button, next_free_row_, 1, 1, 1, Qt::AlignTop);

    widgets_.push_back(*label);
    widgets_.push_back(*push_button);

    next_free_row_++;
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

void Editor::showWidgets()
{
    for (auto &widget : widgets_) {
        widget->show();
    }
}

void Editor::hideWidgets()
{
    for (auto &widget : widgets_) {
        widget->hide();
    }
}

QPalette Editor::colorToPalette(QColor color)
{
    QPalette palette;
    palette.setBrush(QPalette::Button, color);

    return palette;
}
