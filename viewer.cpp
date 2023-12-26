#include <QStatusBar>

#include "viewer.h"
#include "./ui_viewer.h"

viewer::viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::viewer)
{
    ui->setupUi(this);
    QStatusBar *bar = new QStatusBar(this);
    ui->horizontalLayout->addWidget(bar);
}

viewer::~viewer()
{
    delete ui;
}

void viewer::on_reset_view_button_pressed()
{
    ui->display_widget->resetView();
}
