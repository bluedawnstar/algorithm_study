#pragma once

// https://cp-algorithms.com/algebra/polynomial.html

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

/*
   <How to use>

   1. multipoint evaluation

      F(x) = (x - C[0])(x - C[1])...(x - C[n-1])
      Y = { F(x[0]), F(x[1]), ..., F(x[n - 1]) }

      1) build polynomial
         // F(x) = (x - C[0])(x - C[1])...(x - C[n-1])
         vector<algebra::poly<ModInt<int, MOD>>> treeF(4 * N);
         algebra::build(treeF, 1, begin(C), end(C));    // F(x) = treeF.tree[1]

      2) build eval polynomial
         vector<ModInt<int,MOD>> X(n + 1);

         //-- fill X values
         // for (int i = 0; i <= n; i++)
         //    X[i] = i;

         vector<algebra::poly<ModInt<int, MOD>>> evalTree(4 * N);
         algebra::build(evalTree, 1, begin(X), end(X));

      3) calculate Y
         auto Y = evalTree[1].eval(treeF, 1, begin(X), end(X));
*/

namespace algebra {
    // from https://github.com/e-maxx-eng/e-maxx-eng-aux/blob/master/src/polynomial.cpp

    const int INF = 0x3f3f3f3f;
    const int MAGIC = 256;              // threshold for sizes to run the naive algo

//#define USE_NTT

#ifndef USE_NTT
    namespace fft {
        typedef double ftype;
        typedef complex<ftype> point;

        const int MAXN = 1 << 20;       // TODO: tuning point
        const ftype PI = acos(-1);

        point w[MAXN];
        bool initiated = false;

        inline void init() {
            if (!initiated) {
                for (int i = 1; i < MAXN; i <<= 1) {
                    for (int j = 0; j < i; j++) {
                        w[i + j] = polar(ftype(1), PI * j / i);
                    }
                }
                initiated = true;
            }
        }

        template <typename T>
        void fft(const T* in, point* out, int n, int k = 1) {
            if (n == 1) {
                *out = *in;
            } else {
                n >>= 1;
                fft(in    , out    , n, k << 1);
                fft(in + k, out + n, n, k << 1);
                for (int i = 0; i < n; i++) {
                    auto t = out[i + n] * w[i + n];
                    out[i + n] = out[i] - t;
                    out[i] += t;
                }
            }
        }

        // a *= b
        template<typename T>
        void multiplySlow(vector<T>& a, const vector<T>& b) {
            vector<T> res(a.size() + b.size() - 1);
            for (size_t i = 0; i < a.size(); i++) {
                for (size_t j = 0; j < b.size(); j++) {
                    res[i + j] += a[i] * b[j];
                }
            }
            a = res;
        }

        template<typename T>
        void multiply(vector<T>& a, const vector<T>& b) {
            if(min(a.size(), b.size()) < MAGIC) {
                multiplySlow(a, b);
                return;
            }

            init();

            static const int shift = 15;
            static const int mask = (1 << shift) - 1;

            size_t n = a.size() + b.size() - 1;
            size_t size = 1;
            while (size < n)
                size <<= 1;
            size_t sizeMask = size - 1;

            a.resize(size);

            static point A[MAXN], B[MAXN];
            static point C[MAXN], D[MAXN];
            for (size_t i = 0; i < size; i++) {
                A[i] = point(ftype(a[i] & mask), ftype(a[i] >> shift));
                if (i < b.size())
                    B[i] = point(ftype(b[i] & mask), ftype(b[i] >> shift));
                else
                    B[i] = 0;
            }

            fft(A, C, static_cast<int>(size));
            fft(B, D, static_cast<int>(size));
            for (size_t i = 0; i < size; i++) {
                point c0 = C[i] + conj(C[(size - i) & sizeMask]);
                point c1 = C[i] - conj(C[(size - i) & sizeMask]);
                point d0 = D[i] + conj(D[(size - i) & sizeMask]);
                point d1 = D[i] - conj(D[(size - i) & sizeMask]);
                A[i] = c0 * d0 - point(0, 1) * c1 * d1;
                B[i] = c0 * d1 + d0 * c1;
            }

            fft(A, C, static_cast<int>(size));
            fft(B, D, static_cast<int>(size));
            reverse(C + 1, C + size);
            reverse(D + 1, D + size);

            static const T SCALE2(1 << 2 * shift);
            static const T SCALE(1 << shift);

            int t = 4 * static_cast<int>(size);
            for (size_t i = 0; i < size; i++) {
                a[i] = T(llround(real(C[i]) / t))
                     + T(llround(imag(D[i]) / t)) * SCALE
                     + T(llround(imag(C[i]) / t)) * SCALE2;
            }
        }
    }
#else
    namespace ntt {
        const int MAXN_BIT = 20;        // TODO: tuning point
        const int MAXN = 1 << MAXN_BIT;

        const int mod = 998'244'353;    // (119 * 2 ^ 23 + 1)
        const int root = 3;             // primitive root

        template <typename T, typename U>
        T pow(T x, U b) {
            T res = 1;
            while (b > 0) {
                if (b & 1)
                    res *= x;
                x *= x;
                b >>= 1;
            }
            return res;
        }

        template <>
        int pow(int x, int b) {
            long long res = 1;
            long long xx = x;
            while (b > 0) {
                if (b & 1)
                    res = res * xx % mod;
                xx = xx * xx % mod;
                b >>= 1;
            }
            return int(res);
        }

        template <typename T>
        T inverse(T x) {
            return pow(x, mod - 2);
        }

        template <>
        int inverse(int x) {
            return pow(x, mod - 2);
        }

        int w[MAXN], wInv[MAXN];
        int nInv[MAXN_BIT + 1];
        bool initiated = false;

        // counting trailing zeros
        inline int ctz(int x) {
            if (!x)
                return 32;
#ifndef __GNUC__
            return int(_tzcnt_u32(static_cast<unsigned>(x)));
#else
            return __builtin_ctz(static_cast<unsigned>(x));
#endif
        }

        inline void init() {
            if (!initiated) {
                for (int i = 1; i < MAXN; i <<= 1) {
                    int base = pow(root, (mod - 1) / (i * 2));
                    int baseInv = inverse(base);

                    int tw = 1, twInv = 1;
                    for (int j = 0; j < i; j++) {
                        w[i + j] = tw;
                        wInv[i + j] = twInv;

                        tw = int(1ll * tw * base % mod);
                        twInv = int(1ll * twInv * baseInv % mod);
                    }
                }

                for (int i = 0; i <= MAXN_BIT; i++)
                    nInv[i] = inverse(1 << i);

                initiated = true;
            }
        }

        template <typename T>
        void ntt(const T* in, T* out, int n, int k = 1) {
            if (n == 1) {
                *out = *in;
            } else {
                n >>= 1;
                ntt(in, out, n, k << 1);
                ntt(in + k, out + n, n, k << 1);
                for (int i = 0; i < n; i++) {
                    auto t = out[i + n] * w[i + n];
                    out[i + n] = out[i] - t;
                    out[i] += t;
                }
            }
        }

        template <typename T>
        void nttInv(const T* in, T* out, int n, int k = 1) {
            if (n == 1) {
                *out = *in;
            } else {
                n >>= 1;
                nttInv(in, out, n, k << 1);
                nttInv(in + k, out + n, n, k << 1);
                for (int i = 0; i < n; i++) {
                    auto t = out[i + n] * wInv[i + n];
                    out[i + n] = out[i] - t;
                    out[i] += t;
                }
            }
        }

        // a *= b
        template<typename T>
        void multiplySlow(vector<T>& a, const vector<T>& b) {
            vector<T> res(a.size() + b.size() - 1);
            for (size_t i = 0; i < a.size(); i++) {
                for (size_t j = 0; j < b.size(); j++) {
                    res[i + j] += a[i] * b[j];
                }
            }
            swap(a, res);
        }

        template<typename T>
        void multiply(vector<T>& a, vector<T> b) {
            if (min(a.size(), b.size()) < MAGIC) {
                multiplySlow(a, b);
                return;
            }

            init();

            int n = int(a.size()) + int(b.size()) - 1;
            int size = 1;
            while (size < n)
                size <<= 1;

            a.resize(size);
            b.resize(size);

            static T A2[MAXN];
            static T B2[MAXN];

            ntt(a.data(), A2, size);
            ntt(b.data(), B2, size);

            for (int i = 0; i < size; i++)
                A2[i] *= B2[i];

            nttInv(A2, a.data(), size);

            T inv = nInv[ctz(size)];
            for (int i = 0; i < size; i++)
                a[i] *= inv;
        }
    }
    namespace fft = ntt;
#endif

    template <typename T>
    T bpow(T x, size_t n) {
        if (!n)
            return T(1);
        else if (n & 1)
            return x * bpow(x, n - 1);
        else
            return bpow(x * x, n / 2);
    }

    template <typename T>
    T bpow(T x, size_t n, T mod) {
        if (!n)
            return T(1);
        else if (n & 1)
            return x * bpow(x, n - 1, mod) % mod;
        else
            return bpow(x * x % mod, n / 2, mod);
    }

    template <typename T>
    T gcd(const T& a, const T& b) {
        return b == T(0) ? a : gcd(b, a % b);
    }

    // runs in O(r)
    template <typename T>
    T nCr(T n, int r) {
        T res(1);
        for (int i = 0; i < r; i++) {
            res *= n - T(i);
            res /= i + 1;
        }
        return res;
    }

    // low order first
    template <typename T>
    struct poly {
        vector<T> a;

        // get rid of leading zeroes
        void normalize() {
            while (!a.empty() && a.back() == T(0))
                a.pop_back();
        }

        poly() {
        }

        poly(T a0) : a{a0} {
            normalize();
        }

        poly(const vector<T>& rhs) : a(rhs) {
            normalize();
        }

        template <typename U>
        poly(const vector<U>& rhs) {
            a.reserve(rhs.size());
            for (auto& it : rhs)
                a.emplace_back(it);
            normalize();
        }

        poly& operator +=(const poly& rhs) {
            a.resize(max(a.size(), rhs.a.size()));
            for (size_t i = 0; i < rhs.a.size(); i++)
                a[i] += rhs.a[i];
            normalize();
            return *this;
        }

        poly& operator -=(const poly& rhs) {
            a.resize(max(a.size(), rhs.a.size()));
            for (size_t i = 0; i < rhs.a.size(); i++)
                a[i] -= rhs.a[i];
            normalize();
            return *this;
        }
        
        poly operator +(const poly& rhs) const {
            return poly(*this) += rhs;
        }

        poly operator -(const poly& rhs) const {
            return poly(*this) -= rhs;
        }

        // get same polynomial mod x^k
        poly modXK(size_t k) const {
            k = min(k, a.size());
            return vector<T>(begin(a), begin(a) + k);
        }

        // multiply by x^k
        poly mulXK(size_t k) const {
            poly res(*this);
            res.a.insert(begin(res.a), k, 0);
            return res;
        }

        // divide by x^k, dropping coefficients
        poly divXK(size_t k) const {
            k = min(k, a.size());
            return vector<T>(begin(a) + k, end(a));
        }

        // return modXK(r).divXK(l)
        poly substr(size_t l, size_t r) const {
            l = min(l, a.size());
            r = min(r, a.size());
            return vector<T>(begin(a) + l, begin(a) + r);
        }

        // get inverse series mod x^n
        poly inverse(size_t n) const {
            //assert(!isZero());

            poly ans = a[0].inverse();
            size_t a = 1;
            while (a < n) {
                poly C = (ans * modXK(2 * a)).substr(a, 2 * a);
                ans -= (ans * C).modXK(a).mulXK(a);
                a *= 2;
            }
            return ans.modXK(n);
        }

        poly& operator *=(const poly& t) {
            fft::multiply(a, t.a);
            normalize();
            return *this;
        }

        poly operator *(const poly& t) const {
            return poly(*this) *= t;
        }

        // reverses and leaves only n terms
        poly reverse(size_t n, bool rev = false) const {
            poly res(*this);
            if (rev)    // if rev == true then tail goes to head
                res.a.resize(max(n, res.a.size()));
            std::reverse(res.a.begin(), res.a.end());
            return res.modXK(n);
        }

        // when divisor or quotient is small
        // returns quotiend and remainder of a mod b
        pair<poly, poly> divmodSlow(const poly& b) const {
            vector<T> A(a);
            vector<T> res;
            while (A.size() >= b.a.size()) {
                res.push_back(A.back() / b.a.back());
                if (res.back() != T(0)) {
                    for (size_t i = 0; i < b.a.size(); i++)
                        A[A.size() - i - 1] -= res.back() * b.a[b.a.size() - i - 1];
                }
                A.pop_back();
            }
            std::reverse(begin(res), end(res));
            return { res, A };
        }

        // returns quotiend and remainder of a mod b
        pair<poly, poly> divmod(const poly& b) const {
            if (deg() < b.deg())
                return { poly{0}, *this };

            int d = deg() - b.deg();
            if (min(d, b.deg()) < MAGIC)
                return divmodSlow(b);

            poly D = (reverse(d + 1) * b.reverse(d + 1).inverse(d + 1)).modXK(d + 1).reverse(d + 1, true);
            return { D, *this - D * b };
        }

        poly operator /(const poly& t) const {
            return divmod(t).first;
        }

        poly operator %(const poly& t) const {
            return divmod(t).second;
        }

        poly& operator /=(const poly& t) {
            return *this = divmod(t).first;
        }

        poly& operator %=(const poly& t) {
            return *this = divmod(t).second;
        }

        poly& operator *=(const T& x) {
            for(auto& it : a)
                it *= x;
            normalize();
            return *this;
        }

        poly& operator /=(const T& x) {
            for(auto& it : a)
                it /= x;
            normalize();
            return *this;
        }

        poly operator *(const T& x) const {
            return poly(*this) *= x;
        }

        poly operator /(const T& x) const {
            return poly(*this) /= x;
        }

        void print() const {
            for (auto it : a)
                cout << it << ' ';
            cout << endl;
        }

        // evaluates in single point x
        T eval(T x) const {
            T res(0);
            for(int i = int(a.size()) - 1; i >= 0; i--) {
                res *= x;
                res += a[i];
            }
            return res;
        }

        // leading coefficient
        T& lead() {
            return a.back();
        }

        // degree
        int deg() const {
            return a.empty() ? -INF : int(a.size()) - 1;
        }

        // is polynomial zero
        bool isZero() const {
            return a.empty();
        }

        T operator [](int idx) const {
            return (idx >= int(a.size()) || idx < 0) ? T(0) : a[idx];
        }

        // mutable reference at coefficient
        T& coeff(size_t idx) {
            return a[idx];
        }

        bool operator ==(const poly& rhs) const {
            return a == rhs.a;
        }

        bool operator !=(const poly& rhs) const {
            return a != rhs.a;
        }

        template <typename U>
        bool operator ==(const poly<U>& rhs) const {
            if (a.size() != rhs.a.size())
                return false;
            for (size_t i = 0; i < a.size(); i++) {
                if (a[i] != rhs.a[i])
                    return false;
            }
            return true;
        }

        template <typename U>
        bool operator !=(const poly<U>& rhs) const {
            return !operator ==(rhs);
        }

        //---

        // calculate derivative
        poly derivate() {
            vector<T> res;
            for (int i = 1; i <= deg(); i++)
                res.push_back(T(i) * a[i]);
            return res;
        }

        // calculate integral with C = 0
        poly integrate() {
            vector<T> res = {0};
            for (int i = 0; i <= deg(); i++)
                res.push_back(a[i] / T(i + 1));
            return res;
        }

        // Let p(x) = x^k * t(x), return k
        size_t leadingXK() const {
            if (isZero())
                return INF;

            int res = 0;
            while (a[res] == T(0))
                res++;

            return res;
        }

        // calculate log p(x) mod x^n
        poly ln(size_t n) {
            //assert(a[0] == T(1));
            return (derivate().modXK(n) * inverse(n)).integrate().modXK(n);
        }

        // calculate exp p(x) mod x^n
        poly exp(size_t n) {
            if (isZero())
                return T(1);

            //assert(a[0] == T(0));
            poly ans = T(1);
            size_t a = 1;
            while (a < n) {
                poly C = ans.log(a << 1).divXK(a) - substr(a, a << 1);
                ans -= (ans * C).modXK(a).mulXK(a);
                a <<= 1;
            }
            return ans.modXK(n);
        }

        // if k is small
        poly powSlow(size_t k, size_t n) {
            if (!k)
                return T(1);
            else if (k & 1)
                return (*this * powSlow(k - 1, n)).modXK(n);
            else
                return (*this * *this).modXK(n).powSlow(k / 2, n);
        }

        // calculate p^k(n) mod x^n
        poly pow(size_t k, size_t n) {
            if (isZero())
                return *this;
            if (k < MAGIC)
                return powSlow(k, n);

            int i = leadingXK();
            T j = a[i];
            poly t = divXK(i) / j;
            return bpow(j, k) * (t.log(n) * T(k)).exp(n).mulXK(i * k).modXK(n);
        }

        // component-wise multiplication with x^k
        poly mulX(T x) {
            T cur = 1;
            poly res(*this);
            for (int i = 0; i <= deg(); i++) {
                res.coeff(i) *= cur;
                cur *= x;
            }

            return res;
        }

        // component-wise multiplication with x^{k^2}
        poly mulX2(T x) {
            T cur = x;
            T total = 1;
            T xx = x * x;
            poly res(*this);
            for(int i = 0; i <= deg(); i++) {
                res.coeff(i) *= total;
                total *= cur;
                cur *= xx;
            }
            return res;
        }

        // P(1), P(z^2), P(z^4), ..., P(z^2(n-1))
        vector<T> chirpZEven(T z, int n) {
            int m = deg();
            if (isZero())
                return vector<T>(n, 0);

            vector<T> vv(m + n);
            T zi = z.inverse();
            T zz = zi * zi;
            T cur = zi;
            T total = 1;
            for (int i = 0; i <= max(n - 1, m); i++) {
                if (i <= m)
                    vv[m - i] = total;
                if (i < n)
                    vv[m + i] = total;
                total *= cur;
                cur *= zz;
            }
            poly w = (mulX2(z) * vv).substr(m, m + n).mulX2(z);
            vector<T> res(n);
            for (int i = 0; i < n; i++)
                res[i] = w[i];
            return res;
        }

        // P(1), P(z), P(z^2), ..., P(z^(n-1))
        // https://en.wikipedia.org/wiki/Chirp_Z-transform
        vector<T> chirpZ(T z, int n) {
            auto even = chirpZEven(z, (n + 1) / 2);
            auto odd = mulX(z).chirpZEven(z, n / 2);
            vector<T> ans(n);
            for (int i = 0; i < n / 2; i++) {
                ans[2 * i] = even[i];
                ans[2 * i + 1] = odd[i];
            }
            if (n & 1)
                ans[n - 1] = even.back();
            return ans;
        }

        // auxiliary evaluation function
        template <typename iter>
        vector<T> eval(vector<poly>& tree, int node, iter left, iter right) {
            if (right - left == 1) {
                return { eval(*left) };
            } else {
                auto m = left + (right - left) / 2;
                auto A = (*this % tree[2 * node    ]).eval(tree, 2 * node    , left, m);
                auto B = (*this % tree[2 * node + 1]).eval(tree, 2 * node + 1, m, right);
                A.insert(end(A), begin(B), end(B));
                return A;
            }
        }

        // evaluate polynomial in (x1, ..., xn)
        vector<T> eval(const vector<T>& x) {
            int n = int(x.size());
            if (isZero())
                return vector<T>(n, T(0));
            vector<poly> tree(4 * n);
            build(tree, 1, begin(x), end(x));
            return eval(tree, 1, begin(x), end(x));
        }

        // auxiliary interpolation function
        template <typename iter>
        poly interpolate(vector<poly>& tree, int node, iter left, iter right, iter leftY, iter rightY) {
            if (right - left == 1) {
                return { *leftY / a[0] };
            } else {
                auto m  = left + (right - left) / 2;
                auto mY = leftY + (rightY - leftY) / 2;
                auto A = (*this % tree[2 * node    ]).interpolate(tree, 2 * node    , left, m, leftY, mY);
                auto B = (*this % tree[2 * node + 1]).interpolate(tree, 2 * node + 1, m, right, mY, rightY);
                return A * tree[2 * node + 1] + B * tree[2 * node];
            }
        }
    };

    template <typename T>
    poly<T> operator *(const T& a, const poly<T>& b) {
        return b * a;
    }

    // return x^k
    template <typename T>
    poly<T> xk(int k) {
        return poly<T>{1}.mulXK(k);
    }

    // computes resultant of a and b
    template <typename T>
    T resultant(const poly<T>& a, const poly<T>& b) {
        if (b.isZero())
            return 0;
        else if (b.deg() == 0)
            return bpow(b.lead(), a.deg());
        else {
            int pw = a.deg();
            a %= b;
            pw -= a.deg();
            T mul = bpow(b.lead(), pw) * T((b.deg() & a.deg() & 1) ? -1 : 1);
            T ans = resultant(b, a);
            return ans * mul;
        }
    }

    // computes (x - a1)(x - a2)...(x - an) without building tree
    template <typename iter>
    poly<typename iter::value_type> kmul(iter L, iter R) {
        if (R - L == 1)
            return vector<typename iter::value_type>{ -*L, 1 };
        else {
            iter mid = L + (R - L) / 2;
            return kmul(L, mid) * kmul(mid, R);
        }
    }

    // builds evaluation tree for (x - a1)(x - a2)...(x - an)
    template <typename T, typename iter>
    poly<T> build(vector<poly<T>>& res, int node, iter L, iter R) {
        if (R - L == 1)
            return res[node] = vector<T>{ -*L, 1 };
        else {
            iter mid = L + (R - L) / 2;
            return res[node] = build(res, 2 * node, L, mid) * build(res, 2 * node + 1, mid, R);
        }
    }

    // interpolates minimum polynomial from (Xi, Yi) pairs
    template <typename T>
    poly<T> interpolate(const vector<T>& x, const vector<T>& y) {
        int n = int(x.size());
        vector<poly<T>> tree(4 * n);
        return build(tree, 1, begin(x), end(x)).derivate().interpolate(tree, 1, begin(x), end(x), begin(y), end(y));
    }
};

/* <How To Use>

using namespace algebra;

const int MOD = 1000000007;
typedef ModInt<int,MOD> baseT;
typedef poly<baseT> polyT;

*/
