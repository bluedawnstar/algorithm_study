#pragma once

// from https://www.hackerearth.com/challenges/competitive/july-circuits-19/algorithm/something-additive-for-you-4e9342a9/submission/28663751/
// from https://github.com/e-maxx-eng/e-maxx-eng-aux/blob/master/src/polynomial.cpp

template <typename T = long long, int mod = 1000000007>
struct ModInt {
    typedef T   type;
    static const int mod_value = mod;

    T   value;

    constexpr ModInt() : value() {}

    template <typename U>
    ModInt(const U& x) {
        value = normalize(x);
    }

    template <typename U>
    static T normalize(const U& x) {
        T v;
        if (-mod <= x && x < mod)
            v = static_cast<T>(x);
        else
            v = static_cast<T>(x % mod);
        if (v < 0)
            v += mod;
        return v;
    }

    ModInt& operator =(const ModInt& rhs) {
        value = rhs.value;
        return *this;
    }

    template <typename U>
    ModInt& operator =(const U& x) {
        value = normalize(x);
        return *this;
    }

    const T& operator()() const { return value; }
    operator T() const { return value; }
    template <typename U>
    explicit operator U() const { return static_cast<U>(value); }

    ModInt& operator +=(const ModInt& rhs) { if ((value += rhs.value) >= mod) value -= mod; return *this; }
    ModInt& operator -=(const ModInt& rhs) { if ((value -= rhs.value) < 0) value += mod; return *this; }
    template <typename U> ModInt& operator +=(const U& rhs) { return *this += ModInt<T>(rhs); }
    template <typename U> ModInt& operator -=(const U& rhs) { return *this -= ModInt<T>(rhs); }

    ModInt& operator ++() { return *this += 1; }
    ModInt& operator --() { return *this -= 1; }
    ModInt operator ++(int) { ModInt res(*this); *this += 1; return res; }
    ModInt operator --(int) { ModInt res(*this); *this -= 1; return res; }

    ModInt operator -() const { return ModInt(-value); }

    ModInt operator +(const ModInt& rhs) { return ModInt(*this) += rhs; }
    ModInt operator -(const ModInt& rhs) { return ModInt(*this) -= rhs; }
    template <typename U> ModInt operator +(U rhs) { return ModInt(*this) += rhs; }
    template <typename U> ModInt operator -(U rhs) { return ModInt(*this) -= rhs; }

    template <typename U = T>
    typename enable_if<is_same<U, int>::value, ModInt>::type& operator *=(const ModInt& rhs) {
        value = normalize(static_cast<long long>(value) * static_cast<long long>(rhs.value));
        return *this;
    }

    template <typename U = T>
    typename enable_if<is_same<U, long long>::value, ModInt>::type& operator *=(const ModInt& rhs) {
        long long q = static_cast<long long>(static_cast<long double>(value) * rhs.value / mod);
        value = normalize(value * rhs.value - q * mod);
        return *this;
    }

    template <typename U = T>
    typename enable_if<!is_integral<U>::value, ModInt>::type& operator *=(const ModInt& rhs) {
        value = normalize(value * rhs.value);
        return *this;
    }

    ModInt& operator /=(const ModInt& rhs) { return *this *= rhs.inverse(); }

    ModInt operator *(const ModInt& rhs) { return ModInt(*this) *= rhs; }
    ModInt operator /(const ModInt& rhs) { return ModInt(*this) /= rhs; }
    template <typename U> ModInt operator *(U rhs) { return ModInt(*this) *= ModInt<T>(rhs); }
    template <typename U> ModInt operator /(U rhs) { return ModInt(*this) /= ModInt<T>(rhs); }

    const ModInt& abs(const ModInt& v) { return v; }

    bool operator ==(const ModInt& rhs) { return value == rhs.value; }
    bool operator !=(const ModInt& rhs) { return value != rhs.value; }
    template <typename U> bool operator ==(U rhs) { return *this == ModInt<T>(rhs); }
    template <typename U> bool operator !=(U rhs) { return !(*this == ModInt<T>(rhs)); }

    bool operator <(const ModInt& rhs) { return value < rhs.value; }

    ModInt inverse() const {
        return pow(*this, mod - 2);
    }

    template <typename U>
    friend ModInt pow(const ModInt& a, const U& b) {
        //assert(b >= 0);
        ModInt x = a, res = 1;
        U p = b;
        while (p > 0) {
            if (p & 1)
                res *= x;
            x *= x;
            p >>= 1;
        }
        return res;
    }
};

template <typename T, typename U> ModInt<T> operator +(U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) += rhs; }
template <typename T, typename U> ModInt<T> operator -(U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) -= rhs; }
template <typename T, typename U> ModInt<T> operator *(U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) *= rhs; }
template <typename T, typename U> ModInt<T> operator /(U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) /= rhs; }
template <typename T, typename U> bool operator ==(U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) == rhs; }
template <typename T, typename U> bool operator !=(U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) != rhs; }

template <typename T>
bool IsZero(const ModInt<T>& x) {
    return x.value == 0;
}

template <typename T>
string to_string(const ModInt<T>& x) {
    return to_string(x.value);
}


template <typename T, int mod>
void copyFrom(vector<ModInt<T, mod>>& dst, const vector<T>& src) {
    dst.clear();
    dst.reserve(src.size());
    for (auto& it : src)
        dst.emplace_back(it);
}

template <typename T, int mod>
vector<ModInt<T, mod>> copyFrom(const vector<T>& src) {
    vector<ModInt<T, mod>> dst;
    dst.reserve(src.size());
    for (auto& it : src)
        dst.emplace_back(it);
    return dst;
}


template <typename T>
std::ostream& operator <<(std::ostream& os, const ModInt<T>& x) {
    os << x.value;
    return os;
}

template <typename T>
std::istream& operator >>(std::istream& is, ModInt<T>& x) {
    is >> x.value;
    x.value = ModInt<T>::normalize(x.value);
    return is;
}
