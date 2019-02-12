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

    Vec<D, T> &operator+=(const Vec<D, T> &rhs) {
        for (size_t i = 0; i < D; i++) {
            data_[i] += rhs[i];
        }
        return *this;
    }

    Vec<D, T> &operator-=(const Vec<D, T> &rhs) {
        for (size_t i = 0; i < D; i++) {
            data_[i] -= rhs[i];
        }
        return *this;
    }

    template<typename U>
    Vec<D, T> &operator*=(const U &rhs) {
        for (size_t i = 0; i < D; i++) {
            data_[i] *= rhs;
        }
        return *this;
    }

    template<typename U>
    Vec<D, T> &operator/=(const U &rhs) {
        for (size_t i = 0; i < D; i++) {
            data_[i] /= rhs;
        }
        return *this;
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
        Pos<D, T> p;
        for (size_t i = 0; i < D; i++) {
            p[i] = data_[i];
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

    Vec<3, T> &operator+=(const Vec<3, T> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec<3, T> &operator-=(const Vec<3, T> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    template<typename U>
    Vec<3, T> &operator*=(const U &rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    template<typename U>
    Vec<3, T> &operator/=(const U &rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
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
        l[i] *= r[i];
    }
    return l;
}

// Vector sum
template<size_t D, typename T>
Vec<D, T> operator+(Vec<D, T> lhs, const Vec<D, T> &rhs) {
    lhs += rhs;
    return lhs;
}

// Vector difference
template<size_t D, typename T>
Vec<D, T> operator-(Vec<D, T> lhs, const Vec<D, T> &rhs) {
    lhs -= rhs;
    return lhs;
}

// Multiplication by a scalar (left and right)
template<size_t D, typename T, typename U>
Vec<D, T> operator*(Vec<D, T> lhs, const U &rhs) {
    lhs *= rhs;
    return lhs;
}

template<size_t D, typename T, typename U>
Vec<D, T> operator*(const U &lhs, const Vec<D, T> &rhs) {
    return rhs * lhs;
}

// Division by a scalar
template<size_t D, typename T, typename U>
Vec<D, T> operator/(Vec<D, T> lhs, const U &rhs) {
    lhs /= rhs;
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

    Pos<D, T> &operator+=(const Vec<D, T> &rhs) {
        for (size_t i = 0; i < D; i++) {
            data_[i] += rhs[i];
        }
        return *this;
    }

    Pos<D, T> &operator-=(const Vec<D, T> &rhs) {
        for (size_t i = 0; i < D; i++) {
            data_[i] -= rhs[i];
        }
        return *this;
    }

    Vec<D, T> vector() const {
        Vec<D, T> v;
        for (size_t i = 0; i < D; i++) {
            v[i] = data_[i];
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

    Pos<3, T> &operator+=(const Vec<3, T> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Pos<3, T> &operator-=(const Vec<3, T> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
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
    pos += vec;
    return pos;
}

template<size_t D, typename T>
Pos<D, T> operator+(const Vec<D, T> &vec, const Pos<D, T> &pos) {
    return pos + vec;
}

template<size_t D, typename T>
Pos<D, T> operator-(Pos<D, T> pos, const Vec<D, T> &vec) {
    pos -= vec;
    return pos;
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

template<size_t D, typename T>
std::ostream &operator<<(std::ostream &out, const Ray<D, T> &r) {
    out << "Ray(" << r.position << ", " << r.direction << ")";
    return out;
}

/*
template<typename T>
struct Quaternion {
    T t, x, y, z;

    Quaternion<T>()
            : t(), x(), y(), z() {}

    Quaternion<T>(const T &_t, const T &_x, const T &_y, const T &_z)
            : t(_t), x(_x), y(_y), z(_z) {}

    Quaternion<T> &operator+=(const Quaternion<T> &rhs) {
        t += rhs.t;
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Quaternion<T> &operator-=(const Quaternion<T> &rhs) {
        t -= rhs.t;
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    template<typename U>
    Quaternion<T> &operator+=(const U &rhs) {
        t += rhs;
        return *this;
    }

    template<typename U>
    Quaternion<T> &operator-=(const U &rhs) {
        t -= rhs;
        return *this;
    }

    Quaternion<T> &operator*=(const Quaternion<T> &rhs) {
        t = t * rhs.t - x*rhs.x - y*rhs.y - z*rhs.z;
        x = t * rhs.x + x * rhs.t + y * rhs.z - z * rhs.y;
        y = t * rhs.y + y * rhs.t + z * rhs.x - x * rhs.z;
        z = t * rhs.z + z * rhs.t + x * rhs.y - y * rhs.x;
        return *this;
    }

    template<typename U>
    Quaternion<T> &operator*=(const U &rhs) {
        t *= rhs;
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    template<typename U>
    Quaternion<T> &operator/=(const U &rhs) {
        t /= rhs;
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    T length() const {
        return std::sqrt(t * t + x * x + y * y + z * z);
    }

    Quaternion<T> &normalise(T l = 1) {
        *this = *this * (l / length());
        return *this;
    }

    Quaternion<T> unit(T l = 1) const {
        return *this * (l / length());
    }

    Quaternion<T> conjugate() const {
        return Quaternion<T>(t, -x, -y, -z);
    }

    Quaternion<T> pure() const {
        return Quaternion<T>(0, x, y, z);
    }

    Quaternion<T> inverse() const {
        return conjugate() / (t * t + x * x + y * y + z * z);
    }
};

template<typename T>
Quaternion<T> operator+(Quaternion<T> lhs, const Quaternion<T> &rhs) {
    lhs += rhs;
    return lhs;
}

template<typename T>
Quaternion<T> operator-(Quaternion<T> lhs, const Quaternion<T> &rhs) {
    lhs -= rhs;
    return lhs;
}

template<typename T, typename U>
Quaternion<T> operator+(Quaternion<T> lhs, const U &rhs) {
    lhs += rhs;
    return lhs;
}

template<typename T, typename U>
Quaternion<T> operator+(const U &lhs, const Quaternion<T> &rhs) {
    return rhs + lhs;
}

template<typename T, typename U>
Quaternion<T> operator-(Quaternion<T> lhs, const U &rhs) {
    lhs -= rhs;
    return lhs;
}

template<typename T, typename U>
Quaternion<T> operator-(const U &lhs, const Quaternion<T> &rhs) {
    return rhs + -lhs;
}

template<typename T>
Quaternion<T> operator*(Quaternion<T> lhs, const T &rhs) {
    lhs *= rhs;
    return lhs;
}

template<typename T>
Quaternion<T> operator*(const T &lhs, const Quaternion<T> &rhs) {
    return rhs * lhs;
}

template<typename T, typename U>
Quaternion<T> operator/(Quaternion<T> lhs, const U &rhs) {
    lhs /= rhs;
    return lhs;
}

template<typename T>
Quaternion<T> operator-(const Quaternion<T> &q) {
    return q * T(-1);
}

template<typename T>
T inner_product(const Quaternion<T> &lhs, const Quaternion<T> &rhs) {
    return lhs.t * rhs.t + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const Quaternion<T> &q) {
    out << "<" << q.t << " + " << q.x << "i + " << q.y << "j + " << q.z << "k>";
    return out;
}

//
// Return the vector resulting from rotating the given vector around axis by theta radians.
// Assumes that axis is a pure quaternion (with real part 0)
//
template<typename T, typename U, typename V>
Vec<3, T> rotate(const Vec<3, T> &v, const Quaternion<U> &axis, const V theta) {
    const V half_theta = theta / 2;
    const V c = std::cos(half_theta);
    const Quaternion<U> s_a = std::sin(half_theta) * axis;
    const Quaternion<T> q_v(0, v.x, v.y, v.z);
    const Quaternion<T> r = (c + s_a) * q_v * (c - s_a);
    return Vec<3, T>(r.x, r.y, r.z);
}

typedef Quaternion<float> Quatf;
*/