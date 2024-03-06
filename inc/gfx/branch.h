// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file branch.h */

#ifndef BRANCH_H
#define BRANCH_H

#include <vector>

#include "leaf.h"

class RgfCtx;

//! Utility struct to record how fast each branch instance is drawn

//! \sa Branch
struct BranchStatistics
{
    //!  Drawing time of the branch of depth 0
    uint first_branch_render_time_us;

    //!  Drawing time of the branch with highest depth
    uint last_branch_render_time_us;

    //!  A vector containing drawing times of all branches
    std::vector<uint> branch_render_times_us;

    uint num_branches;
};

//!  The branch class contains a collection of leaves that should be drawn simultaneously and in constant relation to each other at each depth level

//! \sa Leaf
//! \sa SpawnPoint
class Branch
{
public:
    //!
    //! \param ctx A pointer to the context
    //!
    Branch(std::weak_ptr<RgfCtx> ctx);

    //!
    //! Draws instances of all leaves onto the view area and the color id buffer for the current depth level
    //!
    //! \param painter A pointer to the painter that paints onto the view area (view buffer)
    //! \param color_id_painter A pointer to painter that paints onto the color id buffer
    //! \param num_iterations How many instances of this branch are left to be drawn
    //! \param stats A reference to this Branch's statistics
    //! \param depth Which consecutive branch the leaf instances are on
    //!
    void draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint num_iterations, BranchStatistics& stats, uint depth);

    //!
    //! Deselects all leaves of the branch
    //!
    void deselect();

    //!
    //! \return The transformations matrix of the Spawn Point leaf
    //!
    QTransform getSpawnPointTransformation();

    //!
    //! Removes a leaf from the branch
    //!
    //! \param leaf A pointer to the leaf to be removed
    //!
    void deleteLeaf(std::shared_ptr<Leaf> leaf);

    //!
    //! Creates a leaf in the branch
    //!
    //! \param shape_type The type of the newly created leaf
    //! \param position Translation of the new leaf's transformation matrix
    //! \param scale Scaling of the new leaf's transformation matrix
    //! \return A pointer to the newly created leaf
    //!
    std::shared_ptr<Leaf> addShape(leaf_type_t shape_type, QPointF position, qreal scale);

private:
    // disable copy and assignment ctors
    Branch(const Branch&) = delete;
    Branch& operator=(const Branch&) = delete;

    std::weak_ptr<RgfCtx> ctx_;

    //!  A vector containing all leaves of the branch
    std::vector<std::shared_ptr<Leaf>> leaves_;
};

#endif // BRANCH_H
