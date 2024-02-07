// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2024  Vesko Milev

#ifndef COMMON_H
#define COMMON_H

#include <numeric>
#include <vector>

const static char *kRgfMimeType = "rgf/leaf";

// TODO: maybe turn this into a class with public IsValid()
enum class leaf_type_t : char {
    spawn_point = 0,
    circle,
    line,
    rectangle,
    path,

    invalid // has to be last member
};

template<class T>
inline T vector_average(std::vector<T> input)
{
    if (input.size() == 0) {
        return 0;
    }
    return static_cast<T>(std::reduce(input.begin(), input.end()) / static_cast<float>(input.size()));
}

#endif // COMMON_H
