// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QGuiApplication>
#include <QScreen>

#include "rgf_ctx.h"

RgfCtx::RgfCtx(DisplayWidget *display_widget, QStatusBar* status_bar) :
    display_widget_(display_widget),
    status_bar_(status_bar),
    leaf_identifier_(std::make_shared<LeafIdentifier>()),
    tree_(nullptr),
    window_buffer_(std::make_shared<QImage>(
        QGuiApplication::primaryScreen()->geometry().width(),
        QGuiApplication::primaryScreen()->geometry().height(),
        QImage::Format_RGB32)),
    color_id_buffer_(std::make_shared<QImage>(
        QGuiApplication::primaryScreen()->geometry().width(),
        QGuiApplication::primaryScreen()->geometry().height(),
        QImage::Format_RGB32)),
    mode_(mode_t::view),
    selected_leaf_(nullptr),
    selected_leaf_depth_(0),
    cumulative_branch_transformations_(QTransform())
{
    assert(window_buffer_ != nullptr && color_id_buffer_ != nullptr && "Couldn't allocate drawing bufffers");
}

std::shared_ptr<RgfCtx> RgfCtx::create(DisplayWidget *display_widget, QStatusBar* status_bar)
{
    // just a wrapper to get to the private ctor
    struct ctor : public RgfCtx {
        ctor(DisplayWidget *display_widget, QStatusBar* status_bar) :
            RgfCtx {display_widget, status_bar} {}
    };

    std::shared_ptr<RgfCtx> ctx = std::make_shared<ctor>(display_widget, status_bar);
    ctx->tree_ = std::make_shared<Tree>(ctx, 100);
    display_widget->setCtx(ctx);

    return ctx;
}

RgfCtx::~RgfCtx()
{
}

void RgfCtx::setNumBranches(uint num_branches)
{
    tree_->setNumBranches(num_branches);
}

void RgfCtx::switchModes()
{
    switch(mode_) {
        case mode_t::view:
            mode_ = mode_t::edit;
            break;
        case mode_t::edit:
            mode_ = mode_t::view;
            break;
        default:
            mode_ = mode_t::view;
            break;
        }

    emit modeSwitched();
}

void RgfCtx::addShape(leaf_type_t shape_type, QPointF position, qreal scale)
{
    std::shared_ptr<Leaf> leaf = tree_->addShape(shape_type, position, scale);
    if (leaf == nullptr) {
        return;
    }

    display_widget_->update();
    display_widget_->updateStatus();

    emit leafSelected(leaf, 0);
}

void RgfCtx::addShape(leaf_type_t shape_type)
{
    View view = display_widget_->getView();
    addShape(shape_type, view.size / 2 - view.offset, view.scale);
}

void RgfCtx::refresh()
{
    display_widget_->update();
}

void RgfCtx::setSelectedLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth)
{
    selected_leaf_ = leaf;
    selected_leaf_depth_ = leaf_depth;
    cumulative_branch_transformations_ = QTransform();

    if (selected_leaf_ != nullptr) {
        QTransform spawn_tfm = tree_->getSpawnPointTransformation();

        for (uint i = 0; i < leaf_depth; i++) {
            cumulative_branch_transformations_ *= spawn_tfm;
        }
    }

    emit leafSelected(leaf, leaf_depth);
}

QPointF RgfCtx::toSelectedBranchSpace(QPointF coordinate)
{
    return cumulative_branch_transformations_.inverted().map(coordinate);
}

void RgfCtx::deleteLeafAction()
{
    if (getMode() != RgfCtx::mode_t::edit) {
        status_bar_->showMessage("You need to be in edit mode in order to delete shapes");
        return;
    }

    auto leaf = getSelectedLeaf();
    if (leaf == nullptr) {
        status_bar_->showMessage("No shape is selected");
        return;
    }

    deleteLeaf(leaf);
    display_widget_->update();
    display_widget_->updateStatus();
}

void RgfCtx::switchModesAction()
{
    switchModes();
    display_widget_->update();
    display_widget_->updateStatus();
}

void RgfCtx::deleteLeaf(std::shared_ptr<Leaf> leaf)
{
    tree_->deleteLeaf(leaf);
    setSelectedLeaf(nullptr, 0);
}
