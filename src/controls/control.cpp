// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "controls/control.h"

Control::Control(std::weak_ptr<RgfCtx> ctx, std::weak_ptr<Leaf> leaf) :
    type_(leaf_type_t::invalid),
    leaf_(leaf),
    ctx_(ctx)
{

}

Control::~Control()
{
}
