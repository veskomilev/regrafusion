// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "controls/control.h"

Control::Control()
{
    type_ = leaf_type_t::invalid;
}

Control::~Control()
{
}
