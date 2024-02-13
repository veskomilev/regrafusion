// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include <vector>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QPalette>
#include <QPushButton>
#include <QWidget>

#include "common.h"
#include "gfx/leaf.h"

#include <QLabel>
#include <QLineEdit>


class Editor : public QObject
{
    Q_OBJECT
public:
    Editor();

    virtual ~Editor();

    void connectToLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth);

    void disconnectFromLeaf();

    enum class state_t {
        disconnected = 0,
        connected
    };

    bool isConnected() const { return state_ == state_t::connected; }

    leaf_type_t getType() const { return type_; }

signals:
    void propertyEdited();

protected:
    virtual void setupWidgets(QGridLayout *grid, uint grid_row) = 0;

    void setupSingleValueControl(QGridLayout *grid, QLabel **label, QLineEdit **line_edit, QString label_text, uint row);

    void setupColorControl(QGridLayout *grid, QLabel **label, QPushButton **push_button, QString label_text, uint row);

    double valueFromLineEdit(QLineEdit *line_editor, double fallback);

    void showWidgets();

    void hideWidgets();

    QPalette colorToPalette(QColor color);

    virtual void update() = 0;

    std::vector<QWidget *> widgets_;

    std::shared_ptr<Leaf> connected_leaf_;

    uint connected_leaf_depth_;

    state_t state_;

    leaf_type_t type_;
};

#endif // EDITOR_H
