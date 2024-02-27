// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef PATH_EDITOR_H
#define PATH_EDITOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "editors/editor.h"

class PathEditor : public Editor
{
public:
    PathEditor(QGridLayout *grid, uint grid_row);

    ~PathEditor();

private:
    void setupWidgets(QGridLayout *grid, uint grid_row);

    void update();

    void setConnectedPathColor();

    QPushButton *color_editor_;

    QLabel *color_label_;
};

#endif // PATH_EDITOR_H
