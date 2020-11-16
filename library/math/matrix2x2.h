#pragma once

template <typename T>
struct Matrix2x2 {
    T a00, a01;
    T a10, a11;

    Matrix2x2& init() {
        a00 = a01 = a10 = a11 = 0;
        return *this;
    }

    Matrix2x2& init(T _a00, T _a01, T _a10, T _a11) {
        a00 = _a00; a01 = _a01;
        a10 = _a10; a11 = _a11;
        return *this;
    }

    Matrix2x2& identity() {
        a00 = a11 = 1;
        a01 = a10 = 0;
        return *this;
    }

    Matrix2x2& operator +=(T x) {
        a00 += x; a01 += x;
        a10 += x; a11 += x;
        return *this;
    }

    Matrix2x2& operator -=(T x) {
        return operator +=(-x);
    }

    Matrix2x2& operator *=(T x) {
        a00 *= x; a01 *= x;
        a10 *= x; a11 *= x;
        return *this;
    }

    Matrix2x2& operator /=(T x) {
        a00 /= x; a01 /= x;
        a10 /= x; a11 /= x;
        return *this;
    }

    Matrix2x2& operator *=(const Matrix2x2& rhs) {
        T t00 = a00 * rhs.a00 + a01 * rhs.a10;
        T t01 = a00 * rhs.a01 + a01 * rhs.a11;
        T t10 = a10 * rhs.a00 + a11 * rhs.a10;
        T t11 = a10 * rhs.a01 + a11 * rhs.a11;
        a00 = t00; a01 = t01;
        a10 = t10; a11 = t11;
        return *this;
    }

    Matrix2x2 operator +(T x) const {
        Matrix2x2 res;
        res.a00 = a00 + x; res.a01 = a01 + x;
        res.a10 = a10 + x; res.a11 = a11 + x;
        return res;
    }

    Matrix2x2 operator -(T x) const {
        return operator +(-x);
    }

    Matrix2x2 operator *(T x) const {
        Matrix2x2 res;
        res.a00 = a00 * x; res.a01 = a01 * x;
        res.a10 = a10 * x; res.a11 = a11 * x;
        return res;
    }

    Matrix2x2 operator /(T x) const {
        Matrix2x2 res;
        res.a00 = a00 / x; res.a01 = a01 / x;
        res.a10 = a10 / x; res.a11 = a11 / x;
        return res;
    }

    Matrix2x2 operator *(const Matrix2x2& rhs) const {
        Matrix2x2 res;
        res.a00 = a00 * rhs.a00 + a01 * rhs.a10;
        res.a01 = a00 * rhs.a01 + a01 * rhs.a11;
        res.a10 = a10 * rhs.a00 + a11 * rhs.a10;
        res.a11 = a10 * rhs.a01 + a11 * rhs.a11;
        return res;
    }

    static const Matrix2x2& getIdentity() {
        static Matrix2x2 id{ T(1), T(0), T(0), T(1) };
        return id;
    }

    //PRECONDITION: n >= 0
    static Matrix2x2 pow(const Matrix2x2& m, long long n) {
        if (n == 1)
            return m;
        else if (n == 0)
            return getIdentity();

        auto t = pow(m, n / 2);
        if (n & 1)
            return m * t * t;
        else
            return t * t;
    }
};
