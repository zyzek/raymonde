#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

template<size_t DIM, typename T>
struct Vec {
    Vec<DIM, T>() {
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

    Vec<DIM, T> &normalise(T l = 1) {
        *this = *this * (l / length());
        return *this;
    }

    Vec<DIM, T> normalised(T l = 1) const {
        return *this * (l / length());
    }

private:

    T data_[DIM];
};

typedef Vec<3, float> Vec3f;

template<typename T>
struct Vec<3, T> {
    Vec<3, T>() : x(T()), y(T()), z(T()) {}

    Vec<3, T>(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

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

    Vec<3, T> &normalise(T l = 1) {
        *this = *this * (l / length());
        return *this;
    }

    Vec<3, T> normalised(T l = 1) const {
        return *this * (l / length());
    }

    T x, y, z;
};

// Cross product specialises for 3-vectors only.
template<typename T>
Vec<3, T> cross(const Vec<3, T> &v1, const Vec<3, T> &v2) {
    return Vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

// Dot product
template<size_t DIM, typename T>
T operator*(const Vec<DIM, T> &lhs, const Vec<DIM, T> &rhs) {
    T result;
    for (size_t i = 0; i < DIM; i++) {
        result += lhs[i] * rhs[i];
    }
    return result;
}

// Vector sum
template<size_t DIM, typename T>
Vec<DIM, T> operator+(Vec<DIM, T> lhs, const Vec<DIM, T> &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

// Vector difference
template<size_t DIM, typename T>
Vec<DIM, T> operator-(Vec<DIM, T> lhs, const Vec<DIM, T> &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] -= rhs[i];
    }
    return lhs;
}

// Multiplication by a scalar (left and right)
template<size_t DIM, typename T, typename U>
Vec<DIM, T> operator*(Vec<DIM, T> lhs, const U &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] *= rhs;
    }
    return lhs;
}

template<size_t DIM, typename T, typename U>
Vec<DIM, T> operator*(const U &lhs, Vec<DIM, T> rhs) {
    for (size_t i = 0; i < DIM; i++) {
        rhs[i] *= lhs;
    }
    return rhs;
}

// Division by a scalar
template<size_t DIM, typename T, typename U>
Vec<DIM, T> operator/(Vec<DIM, T> lhs, const U &rhs) {
    for (size_t i = 0; i < DIM; i++) {
        lhs[i] /= rhs;
    }
    return lhs;
}

// Vector negation
template<size_t DIM, typename T>
Vec<DIM, T> operator-(const Vec<DIM, T> &v) {
    return v * T(-1);
}

// Scalar projection, v onto u.
template<size_t DIM, typename T>
T scalarProjection(const Vec<DIM, T> &v, const Vec<DIM, T> &u) {
    return (v * u) / u.length();
}

// Vector projection, v onto u.
template<size_t DIM, typename T>
Vec<DIM, T> projection(const Vec<DIM, T> &v, const Vec<DIM, T> &u) {
    T uNorm = u.length();
    return ((v * u) / (uNorm * uNorm)) * u;
}

// Stream out operator
template<size_t DIM, typename T>
std::ostream &operator<<(std::ostream &out, const Vec<DIM, T> &v) {
    out << "Vec(";
    if (DIM != 0) {
        for (size_t i = 0; i < DIM - 1; i++) {
            out << v[i] << ", ";
        }
        out << v[DIM - 1];
    }
    out << ")";
    return out;
}

