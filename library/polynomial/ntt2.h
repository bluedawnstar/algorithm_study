#pragma once

// Number Theoretic Transforms
// M = 998244353 (119 * 2^23 + 1), primitive root = 3
//
// [CAUTION]
// It's not working for below 'M's. Use PolyNTT for these
//    - 10^9 + 7
//    - 10^9 + 9
template <int mod, int root, int MaxBitSize = 20>
struct NTT2 {
    static const int MAXN = 1 << MaxBitSize;

    static int w[MAXN], wInv[MAXN];
    static int nInv[MaxBitSize + 1];
    static bool initiated;

    // counting trailing zeros
    static int ctz(int x) {
#ifndef __GNUC__
        return int(_tzcnt_u32(static_cast<unsigned>(x)));
#else
        return __builtin_ctz(static_cast<unsigned>(x));
#endif
    }

    static void init() {
        if (!initiated) {
            for (int i = 1; i < MAXN; i <<= 1) {
                int base = modPow(root, (mod - 1) / (i * 2));
                int baseInv = modInv(base);

                int tw = 1, twInv = 1;
                for (int j = 0; j < i; j++) {
                    w[i + j] = tw;
                    wInv[i + j] = twInv;

                    tw = int(1ll * tw * base % mod);
                    twInv = int(1ll * twInv * baseInv % mod);
                }
            }

            for (int i = 0; i <= MaxBitSize; i++)
                nInv[i] = modInv(1 << i);

            initiated = true;
        }
    }

    static void ntt(const int* in, int* out, int n, int k = 1) {
        if (n == 1) {
            *out = *in;
        } else {
            n >>= 1;
            ntt(in, out, n, k << 1);
            ntt(in + k, out + n, n, k << 1);
            for (int i = 0; i < n; i++) {
                int t = int(1ll * out[i + n] * w[i + n] % mod);

                out[i + n] = out[i] - t;
                if (out[i + n] < 0)
                    out[i + n] += mod;

                out[i] += t;
                if (out[i] >= mod)
                    out[i] -= mod;
            }
        }
    }

    static void nttInv(const int* in, int* out, int n, int k = 1) {
        if (n == 1) {
            *out = *in;
        } else {
            n >>= 1;
            nttInv(in, out, n, k << 1);
            nttInv(in + k, out + n, n, k << 1);
            for (int i = 0; i < n; i++) {
                int t = int(1ll * out[i + n] * wInv[i + n] % mod);

                out[i + n] = out[i] - t;
                if (out[i + n] < 0)
                    out[i + n] += mod;

                out[i] += t;
                if (out[i] >= mod)
                    out[i] -= mod;
            }

            if (k == 1) {
                n <<= 1;
                int inv = nInv[ctz(n)];
                for (int i = 0; i < n; i++)
                    out[i] = int(1ll * out[i] * inv % mod);
            }
        }
    }

    //---

    static int A[MAXN], B[MAXN];
    static int C[MAXN], D[MAXN];

    static vector<int> multiplySlow(const vector<int>& a, const vector<int>& b) {
        vector<int> res(a.size() + b.size() - 1);
        for (int i = 0; i < int(a.size()); i++) {
            for (int j = 0; j < int(b.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * a[i] * b[j]) % mod);
            }
        }
        return res;
    }

    static vector<int> multiply(const vector<int>& a, const vector<int>& b, bool reverseB = false) {
        int sizeA = int(a.size());
        int sizeB = int(b.size());

        if (min(sizeA, sizeB) < 256)
            return multiplySlow(a, b);

        init();

        int n = sizeA + sizeB - 1;
        int size = 1;
        while (size < n)
            size <<= 1;

        memcpy(A, a.data(), sizeof(int) * sizeA);
        if (!reverseB)
            memcpy(B, b.data(), sizeof(int) * sizeB);
        else {
            for (int i = 0, j = sizeB - 1; j >= 0; i++, j--)
                B[j] = b[i];
        }
        memset(A + sizeA, 0, sizeof(int) * (size - sizeA));
        memset(B + sizeB, 0, sizeof(int) * (size - sizeB));

        ntt(A, C, size);
        ntt(B, D, size);

        for (int i = 0; i < size; i++)
            A[i] = int(1ll * C[i] * D[i] % mod);

        vector<int> res(size);
        nttInv(A, res.data(), size);

        res.resize(n);
        normalize(res);
        return res;
    }

    static vector<int> convolute(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        return multiply(x, h, reverseH);
    }

    //--- extended operations

    static void normalize(vector<int>& poly) {
        while (!poly.empty() && poly.back() == 0)
            poly.pop_back();
    }


    static int deg(const vector<int>& poly) {
        return int(poly.size()) - 1;
    }

    // get same polynomial mod x^k
    static vector<int> modXK(vector<int> poly, int k) {
        poly.resize(min(k, int(poly.size())));
        return poly;
    }

    // multiply by x^k
    static vector<int> mulXK(const vector<int>& poly, int k) {
        vector<int> res(k);
        res.insert(res.end(), begin(poly), end(poly));
        return res;
    }

    // divide by x^k, dropping coefficients
    static vector<int> divXK(const vector<int>& poly, int k) {
        k = min(k, int(poly.size()));
        return vector<int>(begin(poly) + k, end(poly));
    }

    // return modXK(r).divXK(l)
    static vector<int> substr(const vector<int>& poly, int l, int r) {
        l = min(l, int(poly.size()));
        r = min(r, int(poly.size()));
        return vector<int>(begin(poly) + l, begin(poly) + r);
    }

    // reverses and leaves only n terms
    static vector<int> reverse(const vector<int>& poly, int n, bool rev = false) {
        vector<int> res(poly);
        if (rev) {   // if rev == true then tail goes to head
            res.resize(max(n, int(res.size())));
            std::reverse(res.begin(), res.end());
        } else {
            std::reverse(res.begin(), res.end());
            res.resize(max(n, int(res.size())));
        }
        return res;
    }


    static vector<int>& add(vector<int>& lhs, const vector<int>& rhs) {
        lhs.resize(max(lhs.size(), rhs.size()));
        for (size_t i = 0; i < rhs.size(); i++) {
            lhs[i] += rhs[i];
            if (lhs[i] >= mod)
                lhs[i] -= mod;
        }
        normalize(lhs);
        return lhs;
    }

    static vector<int> copyAndAdd(const vector<int>& lhs, const vector<int>& rhs) {
        vector<int> res(max(lhs.size(), rhs.size()));
        for (size_t i = 0; i < res.size(); i++) {
            res[i] = lhs[i] + rhs[i];
            if (res[i] >= mod)
                res[i] -= mod;
        }
        normalize(res);
        return res;
    }

    static vector<int>& subtract(vector<int>& lhs, const vector<int>& rhs) {
        lhs.resize(max(lhs.size(), rhs.size()));
        for (size_t i = 0; i < rhs.size(); i++) {
            lhs[i] -= rhs[i];
            if (lhs[i] < 0)
                lhs[i] += mod;
        }
        normalize(lhs);
        return lhs;
    }

    static vector<int> copyAndSubtract(const vector<int>& lhs, const vector<int>& rhs) {
        vector<int> res(lhs);
        res.resize(max(lhs.size(), rhs.size()));

        for (size_t i = 0; i < rhs.size(); i++) {
            res[i] -= rhs[i];
            if (res[i] < 0)
                res[i] += mod;
        }
        normalize(res);
        return res;
    }


    // evaluates in single point x
    static int evaluate(const vector<int>& poly, int x) {
        int res = 0;
        for (int i = int(poly.size()) - 1; i >= 0; i--)
            res = int((poly[i] + 1ll * res * x) % mod);
        return res;
    }


    // when divisor or quotient is small
    // returns quotiend and remainder of a / b
    static pair<vector<int>, vector<int>> divmodSlow(vector<int> a, const vector<int>& b) {
        vector<int> res;
        while (a.size() >= b.size()) {
            res.push_back(a.back() / b.back());
            if (res.back() != 0) {
                for (int i = 0; i < int(b.size()); i++) {
                    a[a.size() - i - 1] = int((a[a.size() - i - 1] - 1ll * res.back() * b[b.size() - i - 1]) % mod);
                    if (a[a.size() - i - 1] < 0)
                        a[a.size() - i - 1] += mod;
                }
            }
            a.pop_back();
        }
        std::reverse(begin(res), end(res));
        return { res, a };
    }

    // returns quotiend and remainder of a / b
    static pair<vector<int>, vector<int>> divmod(const vector<int>& a, const vector<int>& b) {
        if (deg(a) < deg(b))
            return { vector<int>{0}, a };

        int d = deg(a) - deg(b);
        if (min(d, deg(b)) < 256) {
            return divmodSlow(a, b);
        }

        vector<int> D = reverse(modXK(multiply(reverse(a, d + 1), inverse(reverse(b, d + 1), d + 1)), d + 1), d + 1, true);
        return { D, copyAndSubtract(a, multiply(D, b)) };
    }

    //---

    static vector<int> square(const vector<int>& a) {
        int sizeA = int(a.size());
        if (sizeA < 256)
            return multiplySlow(a, a);

        init();

        int n = sizeA * 2 - 1;
        int size = 1;
        while (size < n)
            size <<= 1;

        memcpy(A, a.data(), sizeof(int) * sizeA);
        memset(A + sizeA, 0, sizeof(int) * (size - sizeA));

        ntt(A, B, size);

        for (int i = 0; i < size; i++)
            A[i] = int(1ll * B[i] * B[i] % mod);

        vector<int> res(size);
        nttInv(A, res.data(), size);

        res.resize(n);
        normalize(res);
        return res;
    }


#if 1
    // get inverse series mod x^n
    static vector<int> inverse(const vector<int>& poly, int n) {
        //assert(!poly.empty());

        vector<int> res{ modInv(poly[0]) };
        int a = 1;
        while (a < n) {
            vector<int> C = substr(multiply(res, modXK(poly, 2 * a)), a, 2 * a);
            subtract(res, mulXK(modXK(multiply(res, C), a), a));
            a *= 2;
        }
        return modXK(res, n);
    }
#else
    // low order first
    static vector<int> inverse(const vector<int>& a, int n) {
        //assert(!a.empty());

        vector<int> b = { modInv(a[0]) };
        while (int(b.size()) < n) {
            vector<int> a_cut(a.begin(), a.begin() + min(a.size(), b.size() << 1));
            vector<int> x = multiply(square(b), a_cut);
            b.resize(b.size() << 1);
            for (int i = int(b.size()) >> 1; i < int(min(x.size(), b.size())); i++)
                b[i] = mod - x[i];
        }
        b.resize(n);
        return b;
    }
#endif

    static vector<int> inverse(vector<int> a) {
        return inverse(a, int(a.size()));
    }

    // low order first
    static vector<int> derivate(const vector<int>& poly) {
        vector<int> res;
        res.reserve(poly.size());
        for (int i = 1; i < int(poly.size()); i++)
            res.push_back(int(1ll * i * poly[i] % mod));
        return res;
    }

    // low order first
    static vector<int> integrate(const vector<int>& poly) {
        vector<int> res;
        res.reserve(poly.size() + 1);
        res.push_back(0);
        for (int i = 0; i < int(poly.size()); i++)
            res.push_back(int(1ll * poly[i] * modInv(i + 1) % mod));
        return res;
    }

    // ln f(x) = INTEGRAL f'(x) / f(x)
    // low order first
    static vector<int> ln(vector<int> a) {
        auto A = inverse(a);
        auto B = derivate(a);
        A = multiply(A, B);
        A.resize(a.size());
        return integrate(A);
    }

    // low order first
    static vector<int> exp(vector<int> a) {
        int size = 1;
        while (size < int(a.size()))
            size <<= 1;

        if (size == 1)
            return{ 1 };

        a.resize(size);

        vector<int> dd(a.begin(), a.begin() + (size >> 1));

        vector<int> b = exp(dd);
        b.resize(size);

        vector<int> c = ln(b);
        for (int i = 0; i < size; i++)
            c[i] = int((a[i] - c[i] + mod) % mod);
        c[0]++;

        b = multiply(b, c);
        b.resize(size);

        return b;
    }

    // a[0] != 0
    static vector<int> powFast(const vector<int>& a, int n) { // n >= 0
        auto b = ln(a);
        for (int i = 0; i < int(b.size()); i++)
            b[i] = int(1ll * b[i] * n % mod);
        return exp(b);
    }

    static vector<int> pow(const vector<int>& p, int n, int maxDegree) {
        if (n == 0)
            return{ 1 };

        auto poly = pow(p, n / 2, maxDegree);
        poly = square(poly);
        if (int(poly.size()) > maxDegree + 1)
            poly.resize(maxDegree + 1);

        if (n & 1) {
            poly = multiply(poly, p);
            if (int(poly.size()) > maxDegree + 1)
                poly.resize(maxDegree + 1);
        }

        return poly;
    }


    struct EvaluationTree {
        int N;
        vector<vector<int>> tree;

        // builds evaluation tree for (x - X[0])(x - X[1])...(x - X[n-1])
        void build(const vector<int>& X) {
            N = int(X.size());
            tree = vector<vector<int>>(4 * N);
            if (N > 0)
                dfsBuild(X, 1, 0, N - 1);
        }

        // multipoint evaluation
        vector<int> evaluate(const vector<int>& poly, const vector<int>& X) {
            if (N <= 0)
                return vector<int>(X.size(), 0);
            return dfsEvaluate(poly, X, 1, 0, N - 1);
        }

        // interpolates minimum polynomial from (Xi, Yi) pairs
        vector<int> interpolate(const vector<int>& X, const vector<int>& Y) {
            return dfsInterpolate(derivate(tree[1]), X, Y, 1, 0, N - 1);
        }

    private:
        // builds evaluation tree for (x - a1)(x - a2)...(x - an)
        void dfsBuild(const vector<int>& X, int node, int L, int R) {
            if (L == R)
                tree[node] = vector<int>{ X[L] ? (mod - X[L]) : X[L], 1 };
            else {
                int mid = L + (R - L) / 2;
                dfsBuild(X, 2 * node, L, mid);
                dfsBuild(X, 2 * node + 1, mid + 1, R);
                tree[node] = multiply(tree[2 * node], tree[2 * node + 1]);
            }
        }

        vector<int> dfsEvaluate(const vector<int>& poly, const vector<int>& X, int node, int L, int R) {
            if (R == L) {
                return vector<int>{ NTT2<mod, root, MaxBitSize>::evaluate(poly, X[L]) };
            } else {
                int mid = L + (R - L) / 2;
                auto A = dfsEvaluate(divmod(poly, tree[2 * node]).second, X, 2 * node, L, mid);
                auto B = dfsEvaluate(divmod(poly, tree[2 * node + 1]).second, X, 2 * node + 1, mid + 1, R);
                A.insert(end(A), begin(B), end(B));
                return A;
            }
        }

        vector<int> dfsInterpolate(const vector<int>& poly, const vector<int>& X, const vector<int>& Y, int node, int L, int R) {
            if (R == L) {
                return vector<int>{ int(1ll * Y[L] * modInv(poly[0]) % mod) };
            } else {
                int mid = L + (R - L) / 2;
                auto A = dfsInterpolate(divmod(poly, tree[2 * node]).second, X, Y, 2 * node, L, mid);
                auto B = dfsInterpolate(divmod(poly, tree[2 * node + 1]).second, X, Y, 2 * node + 1, mid + 1, R);
                return copyAndAdd(multiply(A, tree[2 * node + 1]), multiply(B, tree[2 * node]));
            }
        }
    };

//private:
    template <typename T>
    static int modPow(T x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }

    static int modInv(int a) {
        return modPow(a, mod - 2);
    }
};

template <int mod, int root, int MaxBitSize>
int NTT2<mod, root, MaxBitSize>::w[NTT2<mod, root, MaxBitSize>::MAXN];

template <int mod, int root, int MaxBitSize>
int NTT2<mod, root, MaxBitSize>::wInv[NTT2<mod, root, MaxBitSize>::MAXN];

template <int mod, int root, int MaxBitSize>
int NTT2<mod, root, MaxBitSize>::nInv[MaxBitSize + 1];

template <int mod, int root, int MaxBitSize>
bool NTT2<mod, root, MaxBitSize>::initiated = false;

template <int mod, int root, int MaxBitSize>
int NTT2<mod, root, MaxBitSize>::A[NTT2<mod, root, MaxBitSize>::MAXN];

template <int mod, int root, int MaxBitSize>
int NTT2<mod, root, MaxBitSize>::B[NTT2<mod, root, MaxBitSize>::MAXN];

template <int mod, int root, int MaxBitSize>
int NTT2<mod, root, MaxBitSize>::C[NTT2<mod, root, MaxBitSize>::MAXN];

template <int mod, int root, int MaxBitSize>
int NTT2<mod, root, MaxBitSize>::D[NTT2<mod, root, MaxBitSize>::MAXN];
