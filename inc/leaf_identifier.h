// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file leaf_identifier.h */

#ifndef LEAFIDENTIFIER_H
#define LEAFIDENTIFIER_H

#include <map>
#include <memory>
#include <QColor>
#include <QImage>

#include "gfx/leaf.h"

//!  A utility struct/function to help sort a std::map with a key of QColor
struct QColorComparison {
    bool operator()(const QColor& first, const QColor& second) const {
        return first.rgb() < second.rgb();
    }
};

//!  A helper class whose task is to assign a unique color to each leaf.

//!  Each leaf uses its unique color to draw itself in the color id buffer. When a mouse click occurs, the color under the cursor
//!  in the color id buffer is checked and this determines which leaf instance (which leaf at which depth) is selected.
class LeafIdentifier
{
public:
    LeafIdentifier();

    ~LeafIdentifier();

    //!
    //! Assign a unique color to a leaf and mark the color as used
    //!
    //! \param leaf A pointer to the leaf to be assigned color to
    //! \return A unique color that corresponds to the leaf if the operation has been successful, or an invalid color otherwise
    //!
    QColor registerLeaf(std::shared_ptr<Leaf> leaf);

    //!
    //! Unassign a color from a leaf and marked it as unused
    //!
    //! \param leaf A pointer to the leaf to be unassigned color from
    //!
    void unregisterLeaf(std::shared_ptr<Leaf> leaf);

    //!
    //! A polymorphic of getLeaf() with floating point coordinates
    //!
    std::shared_ptr<Leaf> getLeaf(std::shared_ptr<QImage> color_id_buffer, QPointF position, uint& leaf_depth);

    //!
    //! Identifies the leaf under the cursor
    //!
    //! \param color_id_buffer Pointer to the color id buffer
    //! \param position Position of the cursor on the screen
    //! \param leaf_depth A reference to the leaf depth, which is filled in (return parameter)
    //! \return Pointer to the leaf under the cursor (if any) or nullptr if there is none
    //!
    std::shared_ptr<Leaf> getLeaf(std::shared_ptr<QImage> color_id_buffer, QPoint position, uint& leaf_depth);

    QColor getBackgroundColor() const { return kBackgroundColor; }

private:
    //!
    //! Progresses the internal color indicator to the next unused color
    //!
    void goToNextColor();

    //!  A map of all used up colors along with the leaves they respectively identify
    std::map<QColor, std::shared_ptr<Leaf>, QColorComparison> leaf_map_;

    //!  An identicator of the next color to be used as identifier
    QColor next_unused_color_;

    static constexpr QColor kBackgroundColor = QColor(255, 255, 255, 255);

    //!  The first color to be used as identifier
    static constexpr QColor kInitialColor = QColor(0, 0, 0, 255);

    //!  Number of colors other than white, which is the background
    static constexpr size_t kMaxNumLeaves = 256 * 256 * 256 - 1;

    //!  Out of 3 x 8 = 24 rgb bits, the first 14 bits are for the leaf
    static constexpr size_t kLeafMask = 0xFFFC00;

    //!  Out of 3 x 8 = 24 rgb bits, the last 10 bits are for the branch depth
    //! It is necessary to pass around information about depth, since it is needed to calculate branch transformations for the purposes of e.g. dragging a shape around
    //! Total branch transformation is the depth times the spawn point transformation
    static constexpr size_t kDepthMask = 0x3FF;
};

#endif // LEAFIDENTIFIER_H
