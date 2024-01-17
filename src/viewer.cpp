// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#include <functional>
#include <QDrag>
#include <QMimeData>
#include <QStatusBar>
#include <QString>
#include <QToolBar>

#include "viewer.h"
#include "ui_viewer.h"

viewer::viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::viewer)
{
    ui->setupUi(this);

    ctx_ = RgfCtx::create(ui->display_widget, ui->status_bar);
    connect(ctx_.get(), &RgfCtx::modeSwitched, this, &viewer::onRgfCtxModeSwitched);

    ui->display_widget->setStatusBar(ui->status_bar);

    setupToolbar();

    // TODO: should max and min values be defined in code somewhere and passed to the UI?
    uint num_branches = ui->num_branches_spin_box->value();
    ui->num_branches_slider->setValue(num_branches);
    ctx_->setNumBranches(num_branches);

#ifndef QT_DEBUG
    // make this button visible only in debug mode
    ui->switch_buffers->hide();
#endif
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
    ctx_->setNumBranches(value);
    ui->display_widget->update();
}

void viewer::on_num_branches_spin_box_valueChanged(int arg1)
{
    ui->num_branches_slider->setValue(arg1);
    ctx_->setNumBranches(arg1);
    ui->display_widget->update();
}

void viewer::on_switch_buffers_pressed()
{
    ui->display_widget->switchBuffers();
    ui->display_widget->update();
}

void viewer::onRgfCtxModeSwitched()
{
    if (ctx_->getMode() == RgfCtx::mode_t::edit) {
        enableEditModeActions();
    } else {
        disableEditModeActions();
    }
}

void viewer::setupToolbar()
{
    QToolBar* toolbar = new QToolBar("edit");
    this->addToolBar(Qt::LeftToolBarArea, toolbar);

    const QIcon delete_icon = QIcon::fromTheme("process-stop");
    QAction *delete_action = new QAction(delete_icon, "delete selected shape", this);
    delete_action->setShortcuts(QKeySequence::Delete);
    delete_action->setStatusTip("Delete the selected shape");
    connect(delete_action, &QAction::triggered, std::bind(&RgfCtx::deleteLeafAction, ctx_));
    toolbar->addAction(delete_action);

    const QIcon switch_icon = QIcon::fromTheme("media-playlist-shuffle");
    QAction *switch_action = new QAction(switch_icon, "switch modes", this);
    switch_action->setStatusTip("Switch modes");
    connect(switch_action, &QAction::triggered, std::bind(&RgfCtx::switchModesAction, ctx_));
    toolbar->addAction(switch_action);

    toolbar->addSeparator();

    QAction *circle_action = new QAction(QIcon(":/icons/circle.png"), "add a circle", this);
    circle_action->setStatusTip("Drag and drop to add a circle");
    connect(circle_action, &QAction::triggered,
            [this](){
                QMimeData *mimeData = new QMimeData;
                mimeData->setData(kRgfMimeType, QByteArray(kRgfMimeTypeCircle));
                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->exec();
            }
        );
    toolbar->addAction(circle_action);

    edit_mode_actions_.push_back(delete_action);
    edit_mode_actions_.push_back(circle_action);

    disableEditModeActions();
}

void viewer::enableEditModeActions()
{
    for (auto &action : edit_mode_actions_) {
        action->setEnabled(true);
    }
}

void viewer::disableEditModeActions()
{
    for (auto &action : edit_mode_actions_) {
        action->setEnabled(false);
    }
}
