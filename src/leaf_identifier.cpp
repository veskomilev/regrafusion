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
        // TODO: add wraparound functionality if someone ever adds 16 million leaves manually
        return false;
    }

    // some qd hashing - assign a distinct color for debug purposes that is not just shades of black
    size_t hashing = next_unused_color_.rgb();

    hashing *= 11;
    hashing &= 0xFFFFFF;
    hashing *= 577;
    hashing &= 0xFFFFFF;
    hashing *= 997;
    hashing &= 0xFFFFFF;
    hashing *= 1009;
    hashing &= 0xFFFFFF;
    hashing *= 1597;
    hashing &= 0xFFFFFF;
    hashing *= 1741;
    hashing &= 0xFFFFFF;

    QColor display_color;
    display_color.setRgb(hashing);

    fprintf(stderr, "switched to %u - %d\n", next_unused_color_.rgb()  &  0xFFFFFF , display_color.rgb()   );

    leaf->SetColorId(display_color);
    leaf_map_.insert(std::pair<QColor, std::shared_ptr<Leaf>>(display_color, leaf));
    goToNextColor();

    return true;
}

void LeafIdentifier::unregisterLeaf(std::shared_ptr<Leaf> leaf)
{
    QColor id = leaf->GetColorId();
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
