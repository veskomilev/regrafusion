// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file tree.h */

#ifndef TREE_H
#define TREE_H

#include <vector>
#include <QtGlobal>

#include "branch.h"
#include "leaf_identifier.h"

class RgfCtx;

//! Utility struct to record how fast the tree is drawn

//! \sa Tree
struct TreeStatistics
{
    std::vector<uint> render_time_us;
    std::vector<uint> first_branch_render_time_us;
    std::vector<uint> last_branch_render_time_us;
    std::vector<uint> avg_branch_render_time_us;
};

//!  The tree class contains a collection of all branches to be drawn onto the view area.

//! \sa Branch
//! \sa SpawnPoint
class Tree
{
public:
    //!
    //! \param ctx A pointer to the context
    //! \param num_branches_to_draw How many branch instances should be drawn
    //!
    Tree(std::weak_ptr<RgfCtx> ctx, uint num_branches_to_draw);

    //!
    //! Draws all branches onto the view area and the color id buffer
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //! \param color_id_painter A pointer to painter that paints onto the color id buffer
    //! \return Statistics about drawing performance
    //!
    TreeStatistics& draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter);

    //!
    //! Sets how many branch instances should be drawn
    //!
    //! \param num_branches How many branch instances should be drawn
    //!
    void setNumBranches(uint num_branches) { num_branches_to_draw_ = num_branches; }

    uint getNumBranches() { return num_branches_to_draw_; }

    //!
    //! Deselects all branches
    //!
    void deselect();

    //!
    //! \return The transformations matrix of the Spawn Point leaf of the (for now) only Branch
    //!
    QTransform getSpawnPointTransformation();

    //!
    //! Removes a leaf from all branches
    //!
    //! \param leaf A pointer to the leaf to be removed
    //!
    void deleteLeaf(std::shared_ptr<Leaf> leaf);

    //!
    //! Creates a leaf in the (for now) only branch
    //!
    //! \param shape_type The type of the newly created leaf
    //! \param position Translation of the new leaf's transformation matrix
    //! \param scale Scaling of the new leaf's transformation matrix
    //! \return A pointer to the newly created leaf
    //!
    std::shared_ptr<Leaf> addShape(leaf_type_t shape_type, QPointF position, qreal scale);

private:
    // disable copy and assignment ctors
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    std::weak_ptr<RgfCtx> ctx_;

    //!  A vector containing all branches
    std::vector<std::unique_ptr<Branch>> branches_;

    //!  How many branch instances should be drawn
    uint num_branches_to_draw_;

    //!  Statistics about last drawing performance
    TreeStatistics stats_;

    //!  Maximum sample size of each of the vectors of TreeStatistics
    static constexpr uint kMaxStatsSampleSize = 20;
};

#endif // TREE_H
