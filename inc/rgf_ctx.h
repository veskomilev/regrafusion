// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file rgf_ctx.h */

#ifndef RGFCTX_H
#define RGFCTX_H

#include "displaywidget.h"
#include "gfx/tree.h"
#include "leaf_identifier.h"

//!  A context that contains information about program state and holds references to main program components.
class RgfCtx : public QObject
{
Q_OBJECT
public:
    //!
    //! Creates a context singleton. Should be called during and only during program start.
    //!
    //! \param display_widget Pointer to the DisplayWidget
    //! \param status_bar Pointer to the status bar of the window
    //! \return A shared pointer to the context
    //!
    static std::shared_ptr<RgfCtx> create(DisplayWidget *display_widget, QStatusBar* status_bar);

    ~RgfCtx();

    //!
    //! Sets depth of the tree, i.e. how many times a new branch should be drawn (spawned) from the SpawnPoint of the deepest branch yet
    //!
    //! \param num_branches Number of spawned branches that the Tree should have
    //!
    void setNumBranches(uint num_branches);

    const std::shared_ptr<LeafIdentifier> & leafIdentifier() const { return leaf_identifier_; }

    const std::shared_ptr<Tree> & tree() const { return tree_; }

    const std::shared_ptr<QImage> & userViewBuffer() const { return user_view_buffer_; }

    const std::shared_ptr<QImage> & colorIdBuffer() const { return color_id_buffer_; }

    //!  Mode of the program - in view mode maximum frame rate is greater since the color id buffer isn't drawn and some events aren't processed,
    //!  but the tree cannot be edited.
    enum class mode_t {view, edit};

    const mode_t& getMode() const { return mode_; }

    //!
    //! Setter for the selected leaf instance
    //!
    //! \param leaf Pointer to the leaf
    //! \param leaf_depth Depth of the leaf instance
    //!
    void setSelectedLeaf(std::shared_ptr<Leaf> leaf, uint leaf_depth);

    std::shared_ptr<Leaf> getSelectedLeaf() const { return selected_leaf_; }

    uint getSelectedLeafDepth() const { return selected_leaf_depth_; }

    //!
    //! Maps a point from world space to the branch space of the selected leaf instance
    //!
    //! \param coordinate The coordinate to map
    //! \return The mapped coordinate
    //!
    QPointF toSelectedBranchSpace(QPointF coordinate);

    QTransform getSpawnPointTransformation() const { return tree_->getSpawnPointTransformation(); }

    //!
    //! Handler of the delete QAction. Deletes the currently selected leaf
    //!
    void deleteLeafAction();

    //!
    //! Handler of the switch modes QAction. Switches between view mode and edit mode
    //!
    void switchModesAction();

    //!
    //! Switches between view mode and edit mode
    //!
    void switchModes();

    //!
    //! Adds a new leaf to the tree
    //!
    //! \param shape_type Type of the leaf to be added
    //! \param position Position of the leaf to be added
    //! \param scale Scale of the leaf to be added
    //!
    void addShape(leaf_type_t shape_type, QPointF position, qreal scale);

    //!
    //! Polymorphic variant of addShape() with default position and scale
    //!
    //! \param shape_type Type of the leaf to be added
    //!
    void addShape(leaf_type_t shape_type);

    View getView() const { return display_widget_->getView(); }

    //!
    //! Redraws the view area
    //!
    void refresh();

    void setStatusBarMessage(const QString& message) { status_bar_->showMessage(message); }

signals:
    void modeSwitched();

    void leafSelected(std::shared_ptr<Leaf> leaf, uint leaf_depth);

private:
    RgfCtx(DisplayWidget *display_widget, QStatusBar* status_bar);

    //!
    //! Deletes a leaf
    //!
    //! \param leaf Pointer to the leaf to be deleted
    //!
    void deleteLeaf(std::shared_ptr<Leaf> leaf);

    // TODO: figure out a way to replate these with shared pointers
    // as long as the widgets aren't destroyed in run-time (which they aren't) these pointers should never be null
    DisplayWidget *display_widget_;

    QStatusBar* status_bar_;

    std::shared_ptr<LeafIdentifier> leaf_identifier_;

    std::shared_ptr<Tree> tree_;

    //!  The buffer to be drawn to the OpenGL canvas that the user sees
    std::shared_ptr<QImage> user_view_buffer_;

    //!  A buffer containing leaf instances drawn with their respective color ids, used for leaf selection purposes
    std::shared_ptr<QImage> color_id_buffer_;

    mode_t mode_;

    std::shared_ptr<Leaf> selected_leaf_;

    uint selected_leaf_depth_;

    //!  Branch transformation of the selected leaf applied leaf_depth times
    QTransform cumulative_branch_transformations_;
};

#endif // RGFCTX_H
