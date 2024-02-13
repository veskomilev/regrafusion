// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

class LineEditor : public Editor
{
public:
    LineEditor(QGridLayout *grid, uint grid_row);

    ~LineEditor();

private:
    void setupWidgets(QGridLayout *grid, uint grid_row);

    void update();

    void setConnectedLineProperties();

    void setConnectedLineColor();

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

#endif // LINE_EDITOR_H
