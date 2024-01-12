// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef LEAFIDENTIFIER_H
#define LEAFIDENTIFIER_H

#include <map>
#include <memory>
#include <QColor>
#include <QImage>

#include "gfx/leaf.h"


struct QColorComparison {
    bool operator()(const QColor& first, const QColor& second) const {
        return first.rgb() < second.rgb();
    }
};

class LeafIdentifier
{
public:
    LeafIdentifier();

    ~LeafIdentifier();

    bool registerLeaf(std::shared_ptr<Leaf> leaf);

    void unregisterLeaf(std::shared_ptr<Leaf> leaf);

    std::shared_ptr<Leaf> getLeaf(std::shared_ptr<QImage> color_id_buffer, QPointF position);

    std::shared_ptr<Leaf> getLeaf(std::shared_ptr<QImage> color_id_buffer, QPoint position);

    QColor getBackgroundColor() const { return kBackgroundColor; }

private:
    void goToNextColor();

    std::map<QColor, std::shared_ptr<Leaf>, QColorComparison> leaf_map_;

    QColor next_unused_color_;

    static constexpr QColor kBackgroundColor = QColor(255, 255, 255, 255);

    static constexpr QColor kInitialColor = QColor(0, 0, 0, 255);

    // number of colors other than white, which is the background
    static constexpr size_t kMaxNumLeaves = 256 * 256 * 256 - 1;

    // out of 3 x 8 = 24 rgb bits, first 14 bits are for the leaf, and last 10 bits are for the branch depth
    static constexpr size_t kLeafMask = 0xFFFC00;
    static constexpr size_t kDepthMask = 0x3FF;
};

#endif // LEAFIDENTIFIER_H
