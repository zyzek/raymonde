#pragma once

#include "Vector.hpp"

template<size_t DIM, typename T>
struct Ray {
    Vec<DIM, T> base;
    Vec<DIM, T> direction;

    Ray<DIM, T>(const Vec<DIM, T> &b, const Vec<DIM, T> &d)
            : base(b), direction(d) {}
};

typedef Ray<3, float> Ray3f;