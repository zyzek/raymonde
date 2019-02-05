#pragma once

#include "Vector.hpp"

template<size_t DIM, typename T>
struct Ray {
    Pos<DIM, T> position;
    Vec<DIM, T> direction;

    Ray<DIM, T>()
            : position(), direction() {}
            
    Ray<DIM, T>(const Pos<DIM, T> &pos, const Vec<DIM, T> &dir)
            : position(pos), direction(dir) {}
};

typedef Ray<3, float> Ray3f;