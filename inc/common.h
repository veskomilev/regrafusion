// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2024  Vesko Milev

/*! \file common.h
    \brief Contains various definitions and utilities that aren't specific to any class.
*/

#ifndef COMMON_H
#define COMMON_H

#include <numeric>
#include <vector>

//!  Mime type of the custom Drag-and-Drop data used for QMimeData
//!  \sa Leaf::extractType()
//!  \sa shapeWidgetEventFilter::eventFilter()
const static char *kRgfMimeType = "rgf/leaf";

// TODO: maybe turn this into a class with public IsValid()
//!  An enumeration holding all possible leaf types
enum class leaf_type_t : char {
    spawn_point = 0,
    circle,
    line,
    rectangle,
    path,

    //!  A special value representing an 'invalid leaf', which is returned when a function has failed
    invalid // has to be last member
};


//!
//! Averages all the values in a vector
//!
//! \param input The input vector
//! \return An average value of all the values of the vector
//!
template<class T>
inline T vector_average(std::vector<T> input)
{
    if (input.size() == 0) {
        return 0;
    }
    return static_cast<T>(std::reduce(input.begin(), input.end()) / static_cast<float>(input.size()));
}

#endif // COMMON_H
