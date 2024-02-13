// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef RECTANGLE_EDITOR_H
#define RECTANGLE_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

class RectangleEditor : public Editor
{
public:
    RectangleEditor(QGridLayout *grid, uint grid_row);

    ~RectangleEditor();

private:
    void setupWidgets(QGridLayout *grid, uint grid_row);

    void update();

    void setConnectedRectangleProperties();

    void setConnectedRectangleColor();

    QLineEdit *x1_editor_;
    QLineEdit *x2_editor_;
    QLineEdit *y1_editor_;
    QLineEdit *y2_editor_;
    QPushButton *color_editor_;

    QLabel *x1_label_;
    QLabel *x2_label_;
    QLabel *y1_label_;
    QLabel *y2_label_;
    QLabel *color_label_;
};

#endif // RECTANGLE_EDITOR_H
