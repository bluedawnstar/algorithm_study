#pragma once

#include <cmath>
#include <algorithm>
using namespace std;

template <typename T, T mod = 1'000'000'007>
struct ModComplex : public pair<T, T> {
    typedef T   type;
    static const int mod_value = mod;

    ModComplex() : pair<T, T>() {
    }

    ModComplex(T r, T i) : pair<T, T>(normalize(r), normalize(i)) {
    }


    template <typename U>
    static T normalize(U x) {
        T v;
        if (-mod <= x && x < mod)
            v = static_cast<T>(x);
        else
            v = static_cast<T>(x % mod);
        if (v < 0)
            v += mod;
        return v;
    }

    void normalize() {
        this->first = normalize(this->first);
        this->second = normalize(this->second);
    }

    ModComplex conj() const {
        return ModComplex(this->first, neg(this->second));
    }


    ModComplex operator -() const {
        return ModComplex(neg(this->first), neg(this->second));
    }

    template <typename U>
    ModComplex operator +(U rhs) const {
        return ModComplex(normalize(this->first + rhs), this->second);
    }

    template <typename U>
    ModComplex operator -(U rhs) const {
        return ModComplex(normalize(this->first - rhs), this->second);
    }

    template <typename U>
    ModComplex operator *(U rhs) const {
        return ModComplex(normalize(1ll * this->first * rhs), normalize(1ll * this->second * rhs));
    }

    template <typename U>
    ModComplex operator /(U rhs) const {
        T inv = modInv(normalize(rhs));
        return ModComplex(normalize(1ll * this->first * inv), normalize(1ll * this->second * inv));
    }

    ModComplex operator +(const ModComplex& rhs) const {
        return ModComplex(normalize(this->first + rhs.first), normalize(this->second + rhs.second));
    }

    ModComplex operator -(const ModComplex& rhs) const {
        return ModComplex(normalize(this->first - rhs.first), normalize(this->second - rhs.second));
    }

    ModComplex operator *(const ModComplex& rhs) const {
        return ModComplex(normalize(1ll * this->first * rhs.first - 1ll * this->second * rhs.second),
                          normalize(1ll * this->first * rhs.second + 1ll * this->second * rhs.first));
    }

    ModComplex operator /(const ModComplex& rhs) const {
        T inv = modInv(normalize(1ll * rhs.first * rhs.first + 1ll * rhs.second * rhs.second));
        return ModComplex(normalize(1ll * normalize(1ll * this->first * rhs.first + 1ll * this->second * rhs.second) * inv),
                          normalize(1ll * normalize(1ll * this->second * rhs.first - 1ll * this->first * rhs.second) * inv));
    }

    void set(T r, T i) {
        this->first = r;
        this->second = i;
    }

    //T abs() const {
    //    return hypot(this->first, this->second);
    //}

    template <typename U>
    ModComplex& operator +=(U rhs) {
        this->first = normalize(this->first + rhs);
        return *this;
    }

    template <typename U>
    ModComplex& operator -=(U rhs) {
        this->first = normalize(this->first - rhs);
        return *this;
    }

    template <typename U>
    ModComplex& operator *=(U rhs) {
        this->first = normalize(1ll * this->first * rhs);
        this->second = normalize(1ll * this->second * rhs);
        return *this;
    }

    template <typename U>
    ModComplex& operator /=(U rhs) {
        T inv = modInv(normalize(rhs));
        this->first = normalize(1ll * this->first * inv);
        this->second = normalize(1ll * this->second * inv);
        return *this;
    }

    ModComplex& operator +=(const ModComplex& rhs) {
        this->first = normalize(this->first + rhs.first);
        this->second = normalize(this->second + rhs.second);
        return *this;
    }

    ModComplex& operator -=(const ModComplex& rhs) {
        this->first = normalize(this->first - rhs.first);
        this->second = normalize(this->second - rhs.second);
        return *this;
    }

    ModComplex& operator *=(const ModComplex& rhs) {
        T t = normalize(1ll * this->first * rhs.first - 1ll * this->second * rhs.second);
        this->second = normalize(1ll * this->first * rhs.second + 1ll * this->second * rhs.first);
        this->first = t;
        return *this;
    }

    ModComplex& operator /=(const ModComplex& rhs) {
        T inv = modInv(normalize(1ll * rhs.first * rhs.first + 1ll * rhs.second * rhs.second));
        T t = normalize(1ll * normalize(1ll * this->first * rhs.first + 1ll * this->second * rhs.second) * inv);
        this->second = normalize(1ll * normalize(1ll * this->second * rhs.first - 1ll * this->first * rhs.second) * inv);
        this->first = t;
        return *this;
    }


    ModComplex pow(int n) const {
        ModComplex res(1, 0);

        auto x = *this;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * x;
            x = x * x;
        }
        return res;
    }

    ModComplex pow(long long n) const {
        if (this->first == 0 && this->second == 0 && n > 0)
            return ModComplex(0, 0);
        return pow(int(n % (mod - 1)));
    }

private:
    static T neg(T x) {
        T res = -x;
        if (res < 0)
            res += mod;
        return res;
    }

    static T modPow(T x, int n) {
        if (n == 0)
            return 1;

        long long t = x;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return T(res);
    }

    static T modPow(T x, long long n) {
        if (x == 0 && n > 0)
            return 0;
        return modPow(x, int(n % (mod - 1)));
    }

    static T modInv(T x) {
        return modPow(x, mod - 2);
    }
};
