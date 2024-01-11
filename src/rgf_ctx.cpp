// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include <QGuiApplication>
#include <QScreen>

#include "rgf_ctx.h"

RgfCtx::RgfCtx(DisplayWidget *display_widget) :
    leaf_identifier_(std::make_shared<LeafIdentifier>()),
    tree_(std::make_unique<Tree>(leaf_identifier_, 100)),
    window_buffer_(std::make_shared<QImage>(
        QGuiApplication::primaryScreen()->geometry().width(),
        QGuiApplication::primaryScreen()->geometry().height(),
        QImage::Format_RGB32)),
    color_id_buffer_(std::make_shared<QImage>(
        QGuiApplication::primaryScreen()->geometry().width(),
        QGuiApplication::primaryScreen()->geometry().height(),
        QImage::Format_RGB32)),
    mode_(mode_t::view)
{
    assert(window_buffer_ != nullptr && color_id_buffer_ != nullptr && "Couldn't allocate drawing bufffers");
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
}
