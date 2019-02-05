#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

template<size_t D, typename T>
struct Pos;

template<size_t D, typename T>
struct Vec {
    Vec<D, T>() {
        for (size_t i = 0; i < D; i++) {
            data_[i] = T();
        }
    }

    T &operator[](const size_t i) {
        assert(i < D);
        return data_[i];
    }

    const T &operator[](const size_t i) const {
        assert(i < D);
        return data_[i];
    }

    T length() const {
        T result = T();
        for (size_t i = 0; i < D; i++) {
            result += data_[i] * data_[i];
        }
        return std::sqrt(result);
    }

    Vec<D, T> &normalise(T l = 1) {
        *this = *this * (l / length());
        return *this;
    }

    Vec<D, T> unit(T l = 1) const {
        return *this * (l / length());
    }

    Vec<D, T> position() const {
        Pos<D, T> p();
        for (size_t i = 0; i < D; i++) {
            p[i] = this[i];
        }
        return p;
    }

private:

    T data_[D];
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

    Vec<3, T> unit(T l = 1) const {
        return *this * (l / length());
    }

    Vec<3, T> position() const {
        return Pos<3, T>(x, y, z);
    }

    T x, y, z;
};

// Cross product specialises for 3-vectors only.
template<typename T>
Vec<3, T> cross(const Vec<3, T> &v1, const Vec<3, T> &v2) {
    return Vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

// Dot product
template<size_t D, typename T>
T operator*(const Vec<D, T> &lhs, const Vec<D, T> &rhs) {
    T result;
    for (size_t i = 0; i < D; i++) {
        result += lhs[i] * rhs[i];
    }
    return result;
}

// Hadamard (element-wise) product
template<size_t D, typename T>
Vec<D, T> hadamard(Vec<D, T> l, const Vec<D, T> &r) {
    for (size_t i = 0; i < D; i++) {
        l[i] += r[i];
    }
    return l;
}

// Vector sum
template<size_t D, typename T>
Vec<D, T> operator+(Vec<D, T> lhs, const Vec<D, T> &rhs) {
    for (size_t i = 0; i < D; i++) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

// Vector difference
template<size_t D, typename T>
Vec<D, T> operator-(Vec<D, T> lhs, const Vec<D, T> &rhs) {
    for (size_t i = 0; i < D; i++) {
        lhs[i] -= rhs[i];
    }
    return lhs;
}

// Multiplication by a scalar (left and right)
template<size_t D, typename T, typename U>
Vec<D, T> operator*(Vec<D, T> lhs, const U &rhs) {
    for (size_t i = 0; i < D; i++) {
        lhs[i] *= rhs;
    }
    return lhs;
}

template<size_t D, typename T, typename U>
Vec<D, T> operator*(const U &lhs, const Vec<D, T> &rhs) {
    return rhs * lhs;
}

// Division by a scalar
template<size_t D, typename T, typename U>
Vec<D, T> operator/(Vec<D, T> lhs, const U &rhs) {
    for (size_t i = 0; i < D; i++) {
        lhs[i] /= rhs;
    }
    return lhs;
}

// Vector negation
template<size_t D, typename T>
Vec<D, T> operator-(const Vec<D, T> &v) {
    return v * T(-1);
}

// Scalar projection, v onto u.
template<size_t D, typename T>
T scalarProjection(const Vec<D, T> &v, const Vec<D, T> &u) {
    return (v * u) / u.length();
}

// Vector projection, v onto u.
template<size_t D, typename T>
Vec<D, T> projection(const Vec<D, T> &v, const Vec<D, T> &u) {
    T uNorm = u.length();
    return ((v * u) / (uNorm * uNorm)) * u;
}

template<size_t D, typename T>
std::ostream &operator<<(std::ostream &out, const Vec<D, T> &v) {
    out << "Vec(";
    if (D != 0) {
        for (size_t i = 0; i < D - 1; i++) {
            out << v[i] << ", ";
        }
        out << v[D - 1];
    }
    out << ")";
    return out;
}


// Positions are distinct from vectors.
template<size_t D, typename T>
struct Pos {
    Pos<D, T>() {
        for (size_t i = 0; i < D; i++) {
            data_[i] = T();
        }
    }

    T &operator[](const size_t i) {
        assert(i < D);
        return data_[i];
    }

    const T &operator[](const size_t i) const {
        assert(i < D);
        return data_[i];
    }

    Vec<D, T> vector() const {
        Vec<D, T> v();
        for (size_t i = 0; i < D; i++) {
            v[i] = this[i];
        }
        return v;
    }

private:

    T data_[D];
};

typedef Pos<3, float> Pos3f;

template<typename T>
struct Pos<3, T> {
    Pos<3, T>() : x(T()), y(T()), z(T()) {}

    Pos<3, T>(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    T &operator[](const size_t i) {
        assert(i < 3);
        return i <= 0 ? x : i == 1 ? y : z;
    }

    const T &operator[](const size_t i) const {
        assert(i < 3);
        return i <= 0 ? x : i == 1 ? y : z;
    }

    Vec<3, T> vector() const {
        return Vec<3, T>(x, y, z);
    }

    T x, y, z;
};

template<size_t D, typename T>
T distance(const Pos<D, T> &p1, const Pos<D, T> &p2) {
    return (p1 - p2).length();
}

// A difference in positions yields a vector.
template<size_t D, typename T>
Vec<D, T> operator-(const Pos<D, T> &end, const Pos<D, T> &start) {
    Vec<D, T> v;
    for (size_t i = 0; i < D; i++) {
        v[i] = end[i] - start[i];
    }
    return v;
}

// The sum of a vector and a position yields a new position.
template<size_t D, typename T>
Pos<D, T> operator+(Pos<D, T> pos, const Vec<D, T> &vec) {
    for (size_t i = 0; i < D; i++) {
        pos[i] += vec[i];
    }
    return pos;
}

template<size_t D, typename T>
Pos<D, T> operator+(const Vec<D, T> &vec, const Pos<D, T> &pos) {
    return pos + vec;
}

template<size_t D, typename T>
std::ostream &operator<<(std::ostream &out, const Pos<D, T> &v) {
    out << "Pos(";
    if (D != 0) {
        for (size_t i = 0; i < D - 1; i++) {
            out << v[i] << ", ";
        }
        out << v[D - 1];
    }
    out << ")";
    return out;
}
