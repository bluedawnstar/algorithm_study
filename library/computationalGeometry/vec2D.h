#pragma once

#ifndef M_PI
#define M_PI        3.14159265358979323846   // pi
#endif

#ifndef EPSILON
#define EPSILON     1.0e-9
#endif

template <typename T>
inline bool isZero(T x) {
    return x == 0;
}

template<>
inline bool isZero<double>(double x) {
    return fabs(x) < EPSILON;
}

template <typename T>
struct DblType {
    using DblT = typename T;
};

template <>
struct DblType<int> {
    using DblT = long long;
};

// x = first, y = second
template <typename T>
struct Vec2D : public pair<T, T> {
    using DblT = typename DblType<T>::DblT;

    Vec2D() : pair() {
    }

    template <class U>
    Vec2D(const Vec2D<U>& rhs) : pair(rhs) {
    }

    template <class U, class V>
    Vec2D(const pair<U, V>& pr) : pair(rhs) {
    }

    Vec2D(T x, T y) : pair(x, y) {
    }

    Vec2D<T> operator +(const Vec2D<T>& rhs) const {
        return Vec2D{ first + rhs.first, second + rhs.second };
    }

    Vec2D<T> operator -(const Vec2D<T>& rhs) const {
        return Vec2D{ first - rhs.first, second - rhs.second };
    }

    Vec2D<T> operator *(T v) const {
        return Vec2D<T>{ first * v, second * v };
    }
    
    Vec2D<T>& operator +=(const Vec2D<T>& rhs) {
        first += rhs.first;
        second += rhs.second;
        return *this;
    }

    Vec2D<T>& operator -=(const Vec2D<T>& rhs) {
        first -= rhs.first;
        second -= rhs.second;
        return *this;
    }

    Vec2D<T>& operator *=(T v) {
        first *= v;
        second *= v;
        return *this;
    }

    Vec2D<T> operator -() const {
        return Vec2D<T>{ -first, -second };
    }

    double norm() const {
        return hypot(first, second);
    }

    double norm(const Vec2D<T>& rhs) const {
        return hypot(rhs.first - first, rhs.second - second);
    }

    DblT norm2() const {
        return DblT(first) * first + DblT(second) * second;
    }

    DblT norm2(const Vec2D<T>& rhs) const {
        return DblT(rhs.first - first) * (rhs.first - first) + DblT(rhs.second - second) * (rhs.second - second);
    }

    //--- Dot Product ----------------------------------

    // 1) return value > 0  : < 90 degree
    // 2) return value == 0 : == 90 degree
    // 3) return value < 0  : > 90 degree
    DblT dot(const Vec2D<T>& to) const {
        return DblT(first) * to.first + DblT(second) * to.second;
    }

    double angle(const Vec2D<T>& to) const {
        return acos(dot(to) / (norm() * to.norm()));
    }

    //--- Cross Product --------------------------------
    // 1) return value > 0  : < 180 degree
    // 2) return value == 0 : == 180 degree
    // 3) return value < 0  : > 180 degree
    DblT cross(const Vec2D<T>& to) const {
        return DblT(first) * to.second - DblT(second) * to.first;
    }

    bool isCCW(const Vec2D<T>& target) const {
        return cross(target) > 0;
    }

    bool isCW(const Vec2D<T>& target) const {
        return cross(target) < 0;
    }

    bool isCollinear(const Vec2D<T>& target) const {
        return isZero<DblType<T>::DblT>(cross(target));
    }

    Vec2D<double> normalize() const {
        double dist = hypot(first, second);
        return Vec2D<double>{ first / dist, second / dist };
    }

    //--- Projection -----------------------------------

    Vec2D<double> project(const Vec2D<T>& to) const {
        Vec2D<double> r = to.normalize();
        return r * r.dot(Vec2D<double>(*this));
    }
};

inline double normalizeAngle(double angle) {
    return fmod(angle + 2 * M_PI, 2 * M_PI);
}

template <typename T>
inline typename DblType<T>::DblT cross(const Vec2D<T>& base, const Vec2D<T>& a, const Vec2D<T>& b) {
    return TypeConv<T>::DblT(a.first - base.first) * (b.second - base.second) - TypeConv<T>::DblT(a.second - base.second) * (b.first - base.first);
}

template <typename T>
inline bool ccw(const Vec2D<T>& base, const Vec2D<T>& a, const Vec2D<T>& b) {
    return cross(base, a, b) > 0;
}

template <typename T>
inline bool cw(const Vec2D<T>& base, const Vec2D<T>& a, const Vec2D<T>& b) {
    return cross(base, a, b) < 0;
}

//---

inline long long cross(const Vec2D<int>& base, const Vec2D<int>& a, const Vec2D<int>& b) {
    return 1ll * (a.first - base.first) * (b.second - base.second) - 1ll * (a.second - base.second) * (b.first - base.first);
}

inline bool ccw(const Vec2D<int>& base, const Vec2D<int>& a, const Vec2D<int>& b) {
    return cross(base, a, b) > 0;
}

inline bool cw(const Vec2D<int>& base, const Vec2D<int>& a, const Vec2D<int>& b) {
    return cross(base, a, b) < 0;
}
