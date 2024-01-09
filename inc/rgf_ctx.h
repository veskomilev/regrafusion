// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef RGFCTX_H
#define RGFCTX_H

#include "displaywidget.h"

class RgfCtx
{
public:
    RgfCtx(DisplayWidget *display_widget);

    ~RgfCtx();

    void setNumBranches(uint num_branches);

    const std::shared_ptr<LeafIdentifier> & leafIdentifier() const { return leaf_identifier_; }

    const std::shared_ptr<Tree> & tree() const { return tree_; }

    const std::shared_ptr<QImage> & windowBuffer() const { return window_buffer_; }

    const std::shared_ptr<QImage> & colorIdBuffer() const { return color_id_buffer_; }

private:

    std::shared_ptr<LeafIdentifier> leaf_identifier_;

    const std::shared_ptr<Tree> tree_;

    std::shared_ptr<QImage> window_buffer_;

    std::shared_ptr<QImage> color_id_buffer_;
};

#endif // RGFCTX_H