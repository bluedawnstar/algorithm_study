#pragma once

#include "../polynomial/multPoly.h"

// LSB first
struct bigint {
    static const int base = 1000000000;
    static const int baseDigitN = 9;

    vector<int> a;
    int sign;

    bigint() :
        sign(1) {
    }

    explicit bigint(long long v) {
        *this = v;
    }

    explicit bigint(const string& s) {
        operator =(s);
    }

    bigint(const bigint& v) : sign(v.sign), a(v.a) {
    }

    bigint(bigint&& v) : sign(v.sign), a(move(v.a)) {
    }


    bigint& operator =(const bigint& v) {
        sign = v.sign;
        a = v.a;
        return *this;
    }

    bigint& operator =(bigint&& v) {
        sign = v.sign;
        a = move(v.a);
        return *this;
    }

    bigint& operator =(long long v) {
        sign = 1;
        if (v < 0)
            sign = -1, v = -v;
        a.clear();
        for (; v > 0; v = v / base)
            a.push_back(v % base);
        return *this;
    }

    bigint& operator =(const string& s) {
        sign = 1;
        a.clear();
        int pos = 0;
        while (pos < (int)s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-')
                sign = -sign;
            ++pos;
        }
        for (int i = s.size() - 1; i >= pos; i -= baseDigitN) {
            int x = 0;
            for (int j = max(pos, i - baseDigitN + 1); j <= i; j++)
                x = x * 10 + s[j] - '0';
            a.push_back(x);
        }
        trim();
        return *this;
    }


    bool isZero() const {
        return a.empty() || (a.size() == 1 && !a[0]);
    }

    bigint operator -() const {
        bigint res = *this;
        res.sign = -sign;
        return res;
    }

    bigint abs() const {
        bigint res = *this;
        res.sign *= res.sign;
        return res;
    }

    long long longValue() const {
        long long res = 0;
        for (int i = a.size() - 1; i >= 0; i--)
            res = res * base + a[i];
        return res * sign;
    }


    bigint operator +(const bigint& v) const {
        if (sign == v.sign) {
            bigint res = v;

            for (int i = 0, carry = 0; i < (int)max(a.size(), v.a.size()) || carry; ++i) {
                if (i == (int)res.a.size())
                    res.a.push_back(0);
                res.a[i] += carry + (i < (int)a.size() ? a[i] : 0);
                carry = res.a[i] >= base;
                if (carry)
                    res.a[i] -= base;
            }
            return res;
        }
        return move(*this - (-v));
    }

    bigint& operator +=(const bigint& v) {
        *this = move(*this + v);
        return *this;
    }

    bigint operator -(const bigint& v) const {
        if (sign == v.sign) {
            if (abs() >= v.abs()) {
                bigint res = *this;
                for (int i = 0, carry = 0; i < (int)v.a.size() || carry; ++i) {
                    res.a[i] -= carry + (i < (int)v.a.size() ? v.a[i] : 0);
                    carry = res.a[i] < 0;
                    if (carry)
                        res.a[i] += base;
                }
                res.trim();
                return res;
            }
            return -(v - *this);
        }
        return move(*this + (-v));
    }

    bigint& operator -=(const bigint& v) {
        *this = move(*this - v);
        return *this;
    }

    bigint operator *(const bigint& v) const {
        bigint res;
        res.sign = sign * v.sign;
        res.a = multiplyFFT(convertBase(a, baseDigitN, 3), convertBase(v.a, baseDigitN, 3));
        res.a = convertBase(res.a, 3, baseDigitN);
        res.trim();
        return move(res);
    }

    bigint& operator *=(const bigint& v) {
        *this = move(*this * v);
        return *this;
    }
    
    bigint operator /(const bigint& v) const {
        return divMod(*this, v).first;
    }

    bigint& operator /=(const bigint& v) {
        *this = move(*this / v);
        return *this;
    }


    bigint operator %(const bigint& v) const {
        return divMod(*this, v).second;
    }

    bigint& operator %=(const bigint& v) {
        *this = move(*this % v);
        return *this;
    }


    bigint operator *(int v) const {
        bigint res = *this;
        res *= v;
        return move(res);
    }

    bigint& operator *=(int v) {
        if (v < 0)
            sign = -sign, v = -v;
        for (int i = 0, carry = 0; i < (int)a.size() || carry; ++i) {
            if (i == (int)a.size())
                a.push_back(0);
            long long cur = a[i] * (long long)v + carry;
            carry = (int)(cur / base);
            a[i] = (int)(cur % base);
        }
        trim();
        return *this;
    }

    bigint operator /(int v) const {
        bigint res = *this;
        res /= v;
        return move(res);
    }

    bigint& operator /=(int v) {
        if (v < 0)
            sign = -sign, v = -v;
        for (int i = (int)a.size() - 1, rem = 0; i >= 0; --i) {
            long long cur = a[i] + rem * (long long)base;
            a[i] = (int)(cur / v);
            rem = (int)(cur % v);
        }
        trim();
        return *this;
    }

    int operator %(int v) const {
        if (v < 0)
            v = -v;
        int m = 0;
        for (int i = a.size() - 1; i >= 0; --i)
            m = (a[i] + m * (long long)base) % v;
        return m * sign;
    }

    bigint& operator %=(int v) {
        *this = long long(*this % v);
        return *this;
    }


    bool operator <(const bigint &v) const {
        if (sign != v.sign)
            return sign < v.sign;
        if (a.size() != v.a.size())
            return a.size() * sign < v.a.size() * v.sign;
        for (int i = a.size() - 1; i >= 0; i--)
            if (a[i] != v.a[i])
                return a[i] * sign < v.a[i] * sign;
        return false;
    }

    bool operator >(const bigint &v) const {
        return v < *this;
    }

    bool operator <=(const bigint &v) const {
        return !(v < *this);
    }

    bool operator >=(const bigint &v) const {
        return !(*this < v);
    }

    bool operator ==(const bigint &v) const {
        return !(*this < v) && !(v < *this);
    }

    bool operator !=(const bigint &v) const {
        return *this < v || v < *this;
    }


    void trim() {
        while (!a.empty() && a.back() == 0)
            a.pop_back();
        if (a.empty())
            sign = 1;
    }


    static vector<int> convertBase(const vector<int> &a, int oldDigits, int newDigits) {
        vector<long long> p(max(oldDigits, newDigits) + 1);
        p[0] = 1;
        for (int i = 1; i < (int)p.size(); i++)
            p[i] = p[i - 1] * 10;
        vector<int> res;
        long long cur = 0;
        int cur_digits = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            cur += a[i] * p[cur_digits];
            cur_digits += oldDigits;
            while (cur_digits >= newDigits) {
                res.push_back(int(cur % p[newDigits]));
                cur /= p[newDigits];
                cur_digits -= newDigits;
            }
        }
        res.push_back((int)cur);
        while (!res.empty() && res.back() == 0)
            res.pop_back();
        return res;
    }

    bigint multiplySimple(const bigint &v) const {
        bigint res;
        res.sign = sign * v.sign;
        res.a.resize(a.size() + v.a.size());
        for (int i = 0; i < (int)a.size(); ++i)
            if (a[i])
                for (int j = 0, carry = 0; j < (int)v.a.size() || carry; ++j) {
                    long long cur = res.a[i + j] + (long long)a[i] * (j < (int)v.a.size() ? v.a[j] : 0) + carry;
                    carry = (int)(cur / base);
                    res.a[i + j] = (int)(cur % base);
                }
        res.trim();
        return res;
    }

    bigint multiplyKaratsuba(const bigint& v) const {
        vector<int> a6 = convertBase(this->a, baseDigitN, 6);
        vector<int> b6 = convertBase(v.a, baseDigitN, 6);
        vector<long long> a(a6.begin(), a6.end());
        vector<long long> b(b6.begin(), b6.end());
        while (a.size() < b.size())
            a.push_back(0);
        while (b.size() < a.size())
            b.push_back(0);
        while (a.size() & (a.size() - 1))
            a.push_back(0), b.push_back(0);
        vector<long long> c = multiplyKaratsuba(a, b);
        bigint res;
        res.sign = sign * v.sign;
        for (int i = 0, carry = 0; i < (int)c.size(); i++) {
            long long cur = c[i] + carry;
            res.a.push_back((int)(cur % 1000000));
            carry = (int)(cur / 1000000);
        }
        res.a = convertBase(res.a, 6, baseDigitN);
        res.trim();
        return move(res);
    }


    static vector<int> multiplyFFT(const vector<int>& a, const vector<int>& b) {
        auto c = multPolyFFT(a, b);

        vector<int> res;
        res.reserve(c.size() + 2);

        int carry = 0;
        for (int i = 0; i < (int)c.size(); ++i) {
            int t = c[i] + carry;
            carry = t / 1000;
            res.push_back(t % 1000);
        }
        if (carry) {
            res.push_back(carry % 1000);
            carry /= 1000;
        }

        return move(res);
    }

    static vector<long long> multiplyKaratsuba(const vector<long long>& a, const vector<long long>& b) {
        int n = a.size();
        vector<long long> res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    res[i + j] += a[i] * b[j];
            return res;
        }

        int k = n >> 1;
        vector<long long> a1(a.begin(), a.begin() + k);
        vector<long long> a2(a.begin() + k, a.end());
        vector<long long> b1(b.begin(), b.begin() + k);
        vector<long long> b2(b.begin() + k, b.end());

        vector<long long> a1b1 = multiplyKaratsuba(a1, b1);
        vector<long long> a2b2 = multiplyKaratsuba(a2, b2);

        for (int i = 0; i < k; i++)
            a2[i] += a1[i];
        for (int i = 0; i < k; i++)
            b2[i] += b1[i];

        vector<long long> r = multiplyKaratsuba(a2, b2);
        for (int i = 0; i < (int)a1b1.size(); i++)
            r[i] -= a1b1[i];
        for (int i = 0; i < (int)a2b2.size(); i++)
            r[i] -= a2b2[i];

        for (int i = 0; i < (int)r.size(); i++)
            res[i + k] += r[i];
        for (int i = 0; i < (int)a1b1.size(); i++)
            res[i] += a1b1[i];
        for (int i = 0; i < (int)a2b2.size(); i++)
            res[i + n] += a2b2[i];
        return res;
    }

    friend pair<bigint, bigint> divMod(const bigint& a1, const bigint& b1) {
        int norm = base / (b1.a.back() + 1);
        bigint a = a1.abs() * norm;
        bigint b = b1.abs() * norm;
        bigint q, r;
        q.a.resize(a.a.size());

        for (int i = a.a.size() - 1; i >= 0; i--) {
            r *= base;
            r += bigint(a.a[i]);
            int s1 = r.a.size() <= b.a.size() ? 0 : r.a[b.a.size()];
            int s2 = r.a.size() <= b.a.size() - 1 ? 0 : r.a[b.a.size() - 1];
            int d = ((long long)base * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < bigint(0))
                r += b, --d;
            q.a[i] = d;
        }

        q.sign = a1.sign * b1.sign;
        r.sign = a1.sign;
        q.trim();
        r.trim();
        return make_pair(q, r / norm);
    }

    friend bigint gcd(const bigint &a, const bigint &b) {
        return b.isZero() ? a : gcd(b, a % b);
    }
    friend bigint lcm(const bigint &a, const bigint &b) {
        return a / gcd(a, b) * b;
    }
};

inline istream& operator >>(istream& is, bigint& v) {
    string s;
    is >> s;
    v = s;
    return is;
}

inline ostream& operator <<(ostream& os, const bigint& v) {
    if (v.sign == -1)
        os << '-';
    os << (v.a.empty() ? 0 : v.a.back());
    for (int i = (int)v.a.size() - 2; i >= 0; --i)
        os << setw(bigint::baseDigitN) << setfill('0') << v.a[i];
    return os;
}
