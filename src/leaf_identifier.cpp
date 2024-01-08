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

bool LeafIdentifier::RegisterLeaf(std::shared_ptr<Leaf> leaf)
{
    if (next_unused_color_ == kBackgroundColor) {
        // TODO: add wraparound functionality if someone ever adds 16 million leaves manually
        return false;
    }

    leaf->SetColorId(next_unused_color_);
    leaf_map_.insert(std::pair<QColor, std::shared_ptr<Leaf>>(next_unused_color_, leaf));
    GoToNextColor();

    return true;
}

void LeafIdentifier::UnregisterLeaf(std::shared_ptr<Leaf> leaf)
{
    QColor id = leaf->GetColorId();
    leaf_map_.erase(id);
}

std::shared_ptr<Leaf> LeafIdentifier::GetLeaf(std::shared_ptr<QImage> color_id_buffer, QPointF position)
{
    return GetLeaf(color_id_buffer, position.toPoint());
}

std::shared_ptr<Leaf> LeafIdentifier::GetLeaf(std::shared_ptr<QImage> color_id_buffer, QPoint position)
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

void LeafIdentifier::GoToNextColor()
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
