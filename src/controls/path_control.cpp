// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "controls/path_control.h"

PathControl::PathControl()
{
    type_ = leaf_type_t::path;
}

PathControl::~PathControl()
{
}

void PathControl::draw(std::shared_ptr<QPainter> painter, std::shared_ptr<QPainter> color_id_painter, uint depth)
{

}

bool PathControl::handleEvent(QEvent *event)
{
    return false;
}
