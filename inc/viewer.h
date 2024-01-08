// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class viewer;
}
QT_END_NAMESPACE

class viewer : public QMainWindow
{
    Q_OBJECT

public:
    viewer(QWidget *parent = nullptr);
    ~viewer();

private slots:
    void on_reset_view_button_pressed();

    void on_reset_scale_button_pressed();

    void on_num_branches_slider_valueChanged(int value);

    void on_num_branches_spin_box_valueChanged(int arg1);

    void on_switch_buffers_pressed();

private:
    Ui::viewer *ui;
};
#endif // VIEWER_H
