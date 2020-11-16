#pragma once

template <typename T, int mod>
struct Matrix2x2Mod {
    T a00, a01;
    T a10, a11;

    Matrix2x2Mod& init() {
        a00 = a01 = a10 = a11 = 0;
        return *this;
    }

    Matrix2x2Mod& init(T _a00, T _a01, T _a10, T _a11) {
        a00 = _a00; a01 = _a01;
        a10 = _a10; a11 = _a11;
        return *this;
    }

    Matrix2x2Mod& identity() {
        a00 = a11 = 1;
        a01 = a10 = 0;
        return *this;
    }

    Matrix2x2Mod& operator +=(T x) {
        x %= mod;
        a00 += x;
        if (a00 >= mod)
            a00 -= mod;
        a01 += x;
        if (a01 >= mod)
            a01 -= mod;
        a10 += x;
        if (a10 >= mod)
            a10 -= mod;
        a11 += x;
        if (a11 >= mod)
            a11 -= mod;
        return *this;
    }

    Matrix2x2Mod& operator -=(T x) {
        return operator +=(-x);
    }

    Matrix2x2Mod& operator *=(T x) {
        a00 = T(1ll * a00 * x % mod);
        a01 = T(1ll * a01 * x % mod);
        a10 = T(1ll * a10 * x % mod);
        a11 = T(1ll * a11 * x % mod);
        return *this;
    }

    Matrix2x2Mod& operator *=(const Matrix2x2Mod& rhs) {
        T t00 = T((1ll * a00 * rhs.a00 + 1ll * a01 * rhs.a10) % mod);
        T t01 = T((1ll * a00 * rhs.a01 + 1ll * a01 * rhs.a11) % mod);
        T t10 = T((1ll * a10 * rhs.a00 + 1ll * a11 * rhs.a10) % mod);
        T t11 = T((1ll * a10 * rhs.a01 + 1ll * a11 * rhs.a11) % mod);
        a00 = t00; a01 = t01;
        a10 = t10; a11 = t11;
        return *this;
    }

    Matrix2x2Mod operator +(T x) const {
        x %= mod;

        Matrix2x2Mod res;
        res.a00 += x;
        if (res.a00 >= mod)
            res.a00 -= mod;
        res.a01 += x;
        if (res.a01 >= mod)
            res.a01 -= mod;
        res.a10 += x;
        if (res.a10 >= mod)
            res.a10 -= mod;
        res.a11 += x;
        if (res.a11 >= mod)
            res.a11 -= mod;
        return res;
    }

    Matrix2x2Mod operator -(T x) const {
        return operator +(-x);
    }

    Matrix2x2Mod operator *(T x) const {
        Matrix2x2Mod res;
        res.a00 = T(1ll * a00 * x % mod);
        res.a01 = T(1ll * a01 * x % mod);
        res.a10 = T(1ll * a10 * x % mod);
        res.a11 = T(1ll * a11 * x % mod);
        return res;
    }

    Matrix2x2Mod operator *(const Matrix2x2Mod& rhs) const {
        Matrix2x2Mod res;
        res.a00 = T((1ll * a00 * rhs.a00 + 1ll * a01 * rhs.a10) % mod);
        res.a01 = T((1ll * a00 * rhs.a01 + 1ll * a01 * rhs.a11) % mod);
        res.a10 = T((1ll * a10 * rhs.a00 + 1ll * a11 * rhs.a10) % mod);
        res.a11 = T((1ll * a10 * rhs.a01 + 1ll * a11 * rhs.a11) % mod);
        return res;
    }

    static const Matrix2x2Mod& getIdentity() {
        static Matrix2x2Mod id{ T(1), T(0), T(0), T(1) };
        return id;
    }

    //PRECONDITION: n >= 0
    static Matrix2x2Mod pow(const Matrix2x2Mod& m, long long n) {
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
