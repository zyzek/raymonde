#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

template<size_t DIM, typename T>
struct vec {
    vec<DIM, T>() {
        for (size_t i = 0; i < DIM; i++) {
            data_[i] = T();
        }
    }

    T &operator[](const size_t i) {
        assert(i < DIM);
        return data_[i];
    }

    const T &operator[](const size_t i) const {
        assert(i < DIM);
        return data_[i];
    }

    T length() const {
        T result = T();
        for (size_t i = 0; i < DIM; i++) {
            result += data_[i] * data_[i];
        }
        return std::sqrt(result);
    }

    vec<DIM, T> &normalise(T l = 1) {
        *this = *this * (l / length());
        return *this;
    }

    vec<DIM, T> normalised(T l = 1) const {
        return *this * (l / length());
    }

private:

    T data_[DIM];
};

typedef vec<3, float> Vec3f;

template<typename T>
struct vec<3, T> {
    vec<3, T>() : x(T()), y(T()), z(T()) {}

    vec<3, T>(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    T &operator[](const size_t i) {
        assert(i < 3);
        return i <= 0 ? x : i == 1 ? y : z;
    }

    const T &operator[](const size_t i) const {
        assert(i < 3);
        return i <= 0 ? x : i == 1 ? y : z;
    }

    T length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec<3, T> &normalise(T l = 1) {
        *this = *this * (l / length());
        return *this;
    }

    vec<3, T> normalised(T l = 1) const {
        return *this * (l / length());
    }

    T x, y, z;
};

// Cross product specialises for 3-vectors only.
template<typename T>
vec<3, T> cross(const vec<3, T> &v1, const vec<3, T> &v2) {
    return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

// Dot product
template<size_t DIM, typename T>
T operator*(const vec<DIM, T> &lhs, const vec<DIM, T> &rhs) {
    T result;
    for (size_t i = 0; i < DIM; i++) {
        result += lhs[i] * rhs[i];
    }
    return result;
}

// Vector sum
template<size_t DIM, typename T>
vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T> &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

// Vector difference
template<size_t DIM, typename T>
vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T> &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] -= rhs[i];
    }
    return lhs;
}

// Multiplication by a scalar (left and right)
template<size_t DIM, typename T, typename U>
vec<DIM, T> operator*(vec<DIM, T> lhs, const U &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] *= rhs;
    }
    return lhs;
}

template<size_t DIM, typename T, typename U>
vec<DIM, T> operator*(const U &lhs, vec<DIM, T> rhs) {
    for (size_t i = 0; i < DIM; i++) {
        rhs[i] *= lhs;
    }
    return rhs;
}

// Division by a scalar
template<size_t DIM, typename T, typename U>
vec<DIM, T> operator/(vec<DIM, T> lhs, const U &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] /= rhs;
    }
    return lhs;
}

// Vector negation
template<size_t DIM, typename T>
vec<DIM, T> operator-(const vec<DIM, T> &v) {
    return v * T(-1);
}

// Scalar projection, v onto u.
template<size_t DIM, typename T>
T scalarProjection(const vec<DIM, T> &v, const vec<DIM, T> &u) {
    return (v * u) / u.length();
}

// Vector projection, v onto u.
template<size_t DIM, typename T>
vec<DIM, T> projection(const vec<DIM, T> &v, const vec<DIM, T> &u) {
    T uNorm = u.length();
    return ((v * u) / (uNorm * uNorm)) * u;
}

// Stream out operator
template<size_t DIM, typename T>
std::ostream &operator<<(std::ostream &out, const vec<DIM, T> &v) {
    out << "vec(";
    if (DIM != 0) {
        for (size_t i = 0; i < DIM - 1; i++) {
            out << v[i] << ", ";
        }
        out << v[DIM - 1];
    }
    out << ")";
    return out;
}

