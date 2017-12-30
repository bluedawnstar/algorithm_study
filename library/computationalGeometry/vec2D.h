#pragma once

#ifndef M_PI
#define M_PI        3.14159265358979323846   // pi
#endif

#define EPSILON     1.0e-9

template <typename T>
inline bool isZero(T x) {
    return x == 0;
}

template<>
inline bool isZero<double>(double x) {
    return fabs(x) < EPSILON;
}

template <typename T>
struct Vec2D {
    T x, y;

    //Vec2D() { x = 0; y = 0; }
    //Vec2D(T x, T y) { this->x = x; this->y = y; }

    //template <typename U>
    //Vec2D(const Vec2D<U>& rhs) {
    //    x = rhs.x;
    //    y = rhs.y;
    //}

    //template <typename U>
    //Vec2D<T>& operator =(const Vec2D<U>& rhs) {
    //    if (this != &rhs) {
    //        x = rhs.x;
    //        y = rhs.y;
    //    }
    //    return *this;
    //}

    bool operator ==(const Vec2D<T>& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator !=(const Vec2D<T>& rhs) const {
        return !operator ==(rhs);
    }

    bool operator <(const Vec2D<T>& rhs) const {
        return x != rhs.x ? x < rhs.x : y < rhs.y;
    }

    bool operator <=(const Vec2D<T>& rhs) const {
        return operator <(rhs) && operator ==(rhs);
    }

    bool operator >(const Vec2D<T>& rhs) const {
        return !operator <(rhs) && !operator ==(rhs);
    }

    bool operator >=(const Vec2D<T>& rhs) const {
        return !operator <(rhs);
    }

    Vec2D<T> operator +(const Vec2D<T>& rhs) const {
        return Vec2D{ x + rhs.x, y + rhs.y };
    }

    Vec2D<T> operator -(const Vec2D<T>& rhs) const {
        return Vec2D{ x - rhs.x, y - rhs.y };
    }

    Vec2D<T> operator *(T v) const {
        return Vec2D<T>{ x * v, y * v };
    }

    Vec2D<T>& operator +=(const Vec2D<T>& rhs) const {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2D<T>& operator -=(const Vec2D<T>& rhs) const {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2D<T>& operator *=(T v) const {
        x *= v;
        y *= v;
        return *this;
    }

    double norm() const {
        return hypot(x, y);
    }

    double norm(const Vec2D<T>& rhs) const {
        return hypot(rhs.x - x, rhs.y - y);
    }

    T norm2() const {
        return x * x + y * y;
    }

    T norm2(const Vec2D<T>& rhs) const {
        return (rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y);
    }

    //--- Dot Product ----------------------------------

    // 1) return value > 0  : < 90 degree
    // 2) return value == 0 : == 90 degree
    // 3) return value < 0  : > 90 degree
    T dot(const Vec2D<T>& to) const {
        return x * to.x + y * to.y;
    }

    double angle(const Vec2D<T>& to) const {
        return acos(dot(to) / (norm() * to.norm()));
    }

    //--- Cross Product --------------------------------
    // 1) return value > 0  : < 180 degree
    // 2) return value == 0 : == 180 degree
    // 3) return value < 0  : > 180 degree
    T cross(const Vec2D<T>& to) const {
        return x * to.y - y * to.x;
    }

    bool isCCW(const Vec2D<T>& target) const {
        return cross(target) > 0;
    }

    bool isCW(const Vec2D<T>& target) const {
        return cross(target) < 0;
    }

    bool isCollinear(const Vec2D<T>& target) const {
        return isZero<T>(cross(target));
    }

    Vec2D<double> normalize() const {
        double dist = hypot(x, y);
        return Vec2D<double>(x / dist, y / dist);
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
inline T cross(Vec2D<T> base, Vec2D<T> a, Vec2D<T> b) {
    return (a.x - base.x) * (b.y - base.y) - (a.y - base.y) * (b.x - base.x);
}

template <typename T>
inline bool ccw(Vec2D<T> base, Vec2D<T> a, Vec2D<T> b) {
    return cross(base, a, b) > 0;
}

template <typename T>
inline bool cw(Vec2D<T> base, Vec2D<T> a, Vec2D<T> b) {
    return cross(base, a, b) < 0;
}
