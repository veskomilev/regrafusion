// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "leaf_identifier.h"

LeafIdentifier::LeafIdentifier() :
    next_unused_color_(kInitialColor)
{

}

LeafIdentifier::~LeafIdentifier()
{

}

QColor LeafIdentifier::registerLeaf(std::shared_ptr<Leaf> leaf)
{
    if (next_unused_color_ == kBackgroundColor) {
        // TODO: add wraparound functionality if someone ever adds enough leaves manually to 'overflow' the RNG
        return QColor("invalid color");
    }

    size_t rng = next_unused_color_.rgb();

    // out of 3 x 8 = 24 rgb bits, first 14 bits are for the leaf, and last 10 bits are for the branch depth
    // so shift the RNG result left by 10 bits
    rng = (0xDABEDA * rng + 0xBABACECA) % 0x03FFFF;
    rng <<= 10;
    rng &= 0xFFFFFF;

    QColor display_color;
    display_color.setRgb(rng);

    leaf_map_.insert(std::pair<QColor, std::shared_ptr<Leaf>>(display_color, leaf));
    goToNextColor();

    return display_color;
}

void LeafIdentifier::unregisterLeaf(std::shared_ptr<Leaf> leaf)
{
    QColor id = leaf->getColorId();
    leaf_map_.erase(id);
}

std::shared_ptr<Leaf> LeafIdentifier::getLeaf(std::shared_ptr<QImage> color_id_buffer, QPointF position, uint& leaf_depth)
{
    return getLeaf(color_id_buffer, position.toPoint(), leaf_depth);
}

std::shared_ptr<Leaf> LeafIdentifier::getLeaf(std::shared_ptr<QImage> color_id_buffer, QPoint position, uint& leaf_depth)
{
    QColor pixel_color = color_id_buffer->pixelColor(position);

    // apply leaf mask - ignore branch depth information
    QColor color_id(pixel_color.rgb() & kLeafMask);

    if (color_id == QColor::Invalid) {
        leaf_depth = 0;
        return nullptr;
    }

    if (leaf_map_.find(color_id) == leaf_map_.end()) {
        leaf_depth = 0;
        return nullptr;
    }

    leaf_depth = pixel_color.rgb() & kDepthMask;

    return leaf_map_[color_id];
}

void LeafIdentifier::goToNextColor()
{
    QRgb rgb = next_unused_color_.rgb();

    // per QT documentation, this is an unsigned int with hex structure of AARRGGBB
    rgb++;

    if (rgb == kBackgroundColor.rgb()) {
        next_unused_color_ = kInitialColor;
    } else {
        next_unused_color_.setRgb(rgb);
    }
}
