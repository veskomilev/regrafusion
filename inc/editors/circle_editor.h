// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef CIRCLE_EDITOR_H
#define CIRCLE_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

class CircleEditor : public Editor
{
public:
    CircleEditor(QGridLayout *grid, uint grid_row);

    ~CircleEditor();

private:
    void setupWidgets(QGridLayout *grid, uint grid_row);

    void update();

    void setConnectedCircleProperties();

    QLineEdit *radius_editor_;
    QLabel *radius_label_;
};

#endif // CIRCLE_EDITOR_H
