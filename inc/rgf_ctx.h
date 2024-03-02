// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef RGFCTX_H
#define RGFCTX_H

#include "displaywidget.h"
#include "gfx/tree.h"
#include "leaf_identifier.h"

class RgfCtx : public QObject
{
Q_OBJECT
public:
    static std::shared_ptr<RgfCtx> create(DisplayWidget *display_widget, QStatusBar* status_bar);

    ~RgfCtx();

    void setNumBranches(uint num_branches);

    const std::shared_ptr<LeafIdentifier> & leafIdentifier() const { return leaf_identifier_; }

    const std::shared_ptr<Tree> & tree() const { return tree_; }

    const std::shared_ptr<QImage> & windowBuffer() const { return window_buffer_; }

    const std::shared_ptr<QImage> & colorIdBuffer() const { return color_id_buffer_; }

    enum class mode_t {view, edit};

    const mode_t& getMode() const { return mode_; }

    void setSelectedLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth);

    std::shared_ptr<Leaf> getSelectedLeaf() const { return selected_leaf_; }

    uint getSelectedLeafDepth() const { return selected_leaf_depth_; }

    QPointF toSelectedBranchSpace(QPointF coordinate);

    QTransform getSpawnPointTransformation() const { return tree_->getSpawnPointTransformation(); }

    void deleteLeafAction();

    void switchModesAction();

    void switchModes();

    void addShape(leaf_type_t shape_type, QPointF position, qreal scale);

    void addShape(leaf_type_t shape_type);

    View getView() const { return display_widget_->getView(); }

    void refresh();

    void setStatusBarMessage(const QString& message) { status_bar_->showMessage(message); }

signals:
    void modeSwitched();

    void leafSelected(std::shared_ptr<Leaf> leaf, uint leaf_depth);

private:
    RgfCtx(DisplayWidget *display_widget, QStatusBar* status_bar);

    void deleteLeaf(std::shared_ptr<Leaf> leaf);

    // TODO: figure out a way to replate these with shared pointers
    // as long as the widgets aren't destroyed in run-time (which they aren't) these pointers should never be null
    DisplayWidget *display_widget_;

    QStatusBar* status_bar_;

    std::shared_ptr<LeafIdentifier> leaf_identifier_;

    std::shared_ptr<Tree> tree_;

    std::shared_ptr<QImage> window_buffer_;

    std::shared_ptr<QImage> color_id_buffer_;

    mode_t mode_;

    std::shared_ptr<Leaf> selected_leaf_;

    uint selected_leaf_depth_;

    QTransform cumulative_branch_transformations_;
};

#endif // RGFCTX_H
