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

bool LeafIdentifier::registerLeaf(std::shared_ptr<Leaf> leaf)
{
    if (next_unused_color_ == kBackgroundColor) {
        // TODO: add wraparound functionality if someone ever adds enough leaves manually to 'overflow' the RNG
        return false;
    }

    size_t rng = next_unused_color_.rgb();

    rng = (0xDABEDA * rng + 0xBABACECA) % 0x03FFFF;
    rng <<= 10;
    rng &= 0xFFFFFF;

    QColor display_color;
    display_color.setRgb(rng);

    leaf->setColorId(display_color);
    leaf_map_.insert(std::pair<QColor, std::shared_ptr<Leaf>>(display_color, leaf));
    goToNextColor();

    return true;
}

void LeafIdentifier::unregisterLeaf(std::shared_ptr<Leaf> leaf)
{
    QColor id = leaf->getColorId();
    leaf_map_.erase(id);
}

std::shared_ptr<Leaf> LeafIdentifier::getLeaf(std::shared_ptr<QImage> color_id_buffer, QPointF position)
{
    return getLeaf(color_id_buffer, position.toPoint());
}

std::shared_ptr<Leaf> LeafIdentifier::getLeaf(std::shared_ptr<QImage> color_id_buffer, QPoint position)
{
    QColor color_id = color_id_buffer->pixelColor(position);

    if (color_id == QColor::Invalid) {
        return nullptr;
    }

    if (leaf_map_.find(color_id) == leaf_map_.end()) {
        return nullptr;
    }

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
