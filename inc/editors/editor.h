// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include <vector>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QWidget>

#include "gfx/leaf.h"

#include <QLabel>
#include <QLineEdit>

class Editor
{
public:
    Editor();

    virtual ~Editor();

    virtual void setupWidgets(QGridLayout *grid) = 0;

    virtual void showWidgets(std::shared_ptr<Leaf> leaf, uint leaf_depth) = 0;

    virtual void hideWidgets() = 0;

protected:
    void setupSingleValueControl(QGridLayout *grid, QLabel **label, QLineEdit **line_edit, QString label_text, uint row);

    std::vector<QWidget *> widgets_;

    QDoubleValidator double_validator_;
};

#endif // EDITOR_H
