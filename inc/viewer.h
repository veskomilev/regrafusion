// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023  Vesko Milev

#ifndef VIEWER_H
#define VIEWER_H

#include <memory>
#include <QMainWindow>
#include <string>
#include <vector>

#include "rgf_ctx.h"

#include "editors/editor.h"
#include "editors/circle_editor.h"
#include "editors/line_editor.h"
#include "editors/transform_editor.h"

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

    void onRgfCtxModeSwitched();

    void onLeafSelected(std::shared_ptr<Leaf> leaf, uint leaf_depth);

private:
    void setupToolbar();

    void setupEditors();

    int getNextFreeRowInGridLayout();

    void enableEditModeActions();

    void disableEditModeActions();

    void addEditModeAction(QToolBar* toolbar, std::string resource_path, std::string description, std::string status_tip, leaf_type_t type);

    Ui::viewer *ui;
    std::shared_ptr<RgfCtx> ctx_;

    std::vector<QAction *> edit_mode_actions_;

    std::shared_ptr<TransformEditor> tfm_editor_;
    std::shared_ptr<CircleEditor> circle_editor_;
    std::shared_ptr<LineEditor> line_editor_;
    std::vector<std::shared_ptr<Editor>> editors_;
};
#endif // VIEWER_H
