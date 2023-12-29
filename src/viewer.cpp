// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <QStatusBar>
#include <QString>

#include "viewer.h"
#include "./ui_viewer.h"

viewer::viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::viewer)
{
    ui->setupUi(this);
    ui->display_widget->setStatusBar(ui->status_bar);

    // TODO: should max and min values be defined in code somewhere and passed to the UI?
    uint num_branches = ui->num_branches_spin_box->value();
    ui->num_branches_slider->setValue(num_branches);
    ui->display_widget->setNumBranches(num_branches);
}

viewer::~viewer()
{
    delete ui;
}

void viewer::on_reset_view_button_pressed()
{
    ui->display_widget->resetViewPosition();
}

void viewer::on_reset_scale_button_pressed()
{
    ui->display_widget->resetViewScale();
}

void viewer::on_num_branches_slider_valueChanged(int value)
{
    ui->num_branches_spin_box->setValue(value);
    ui->display_widget->setNumBranches(value);
    ui->display_widget->update();
}

void viewer::on_num_branches_spin_box_valueChanged(int arg1)
{
    ui->num_branches_slider->setValue(arg1);
    ui->display_widget->setNumBranches(arg1);
    ui->display_widget->update();
}
