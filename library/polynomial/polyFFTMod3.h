#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

/*
   <How to use>

   1. multipoint evaluation

      F(x) = (x - C[0])(x - C[1])...(x - C[n-1])
      Y = { F(x[0]), F(x[1]), ..., F(x[n - 1]) }

      1) build polynomial
         // F(x) = (x - C[0])(x - C[1])...(x - C[n-1])
         PolyFFTMod3<MOD>::EvaluationTree F;
         F.build(C);    // F(x) = F.tree[1]

      2) build eval polynomial
         vector<int> X(n + 1);
         // fill X values
         // ex) iota(X.begin(), X.end(), 0);

         PolyFFTMod3<MOD>::EvaluationTree evalTree;
         evalTree.build(X);

      3) calculate Y
         auto Y = evalTree.evaluate(F.tree[1], X);
*/

template <int mod, int MaxN = (1 << 20)>
struct PolyFFTMod3 {
    static pair<double, double> w[MaxN];
    static bool initialized;

    static void init() {
        if (!initialized) {
            for (int i = 1; i < MaxN; i <<= 1) {
                for (int j = 0; j < i; j++) {
                    w[i + j].first = cos(M_PI * j / i);
                    w[i + j].second = sin(M_PI * j / i);
                }
            }
            initialized = true;
        }
    }

    static void fft(const pair<double, double>* in, pair<double, double>* out, int n, int k = 1) {
        if (n == 1) {
            *out = *in;
        } else {
            n >>= 1;
            fft(in, out, n, k << 1);
            fft(in + k, out + n, n, k << 1);
            for (int i = 0; i < n; i++) {
                pair<double, double> t(out[i + n].first * w[i + n].first - out[i + n].second * w[i + n].second,
                    out[i + n].second * w[i + n].first + out[i + n].first * w[i + n].second);

                out[i + n].first = out[i].first - t.first;
                out[i + n].second = out[i].second - t.second;
                out[i].first += t.first;
                out[i].second += t.second;
            }
        }
    }

    static void fftInv(const pair<double, double>* in, pair<double, double>* out, int n, int k = 1) {
        if (n == 1) {
            *out = *in;
        } else {
            n >>= 1;
            fftInv(in, out, n, k << 1);
            fftInv(in + k, out + n, n, k << 1);
            for (int i = 0; i < n; i++) {
                pair<double, double> t(out[i + n].first * w[i + n].first + out[i + n].second * w[i + n].second,
                    out[i + n].second * w[i + n].first - out[i + n].first * w[i + n].second);

                out[i + n].first = out[i].first - t.first;
                out[i + n].second = out[i].second - t.second;
                out[i].first += t.first;
                out[i].second += t.second;
            }
        }

        if (k == 1) {
            for (int i = 0; i < n; i++) {
                out[i].first /= n;
                out[i].second /= n;
            }
        }
    }

    //---

    static pair<double, double> A[MaxN], B[MaxN];
    static pair<double, double> C[MaxN], D[MaxN];

    static vector<int> multiplySlow(const vector<int>& a, const vector<int>& b) {
        vector<int> res(a.size() + b.size() - 1);
        for (int i = 0; i < int(a.size()); i++) {
            for (int j = 0; j < int(b.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * a[i] * b[j]) % mod);
            }
        }
        return res;
    }

    static vector<int> multiply(const vector<int>& left, const vector<int>& right) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

        if (min(sizeL, sizeR) <= 256)
            return multiplySlow(left, right);

        init();

        static const int shift = 15;
        static const int mask = (1 << shift) - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;
        int sizeMask = size - 1;

        for (int i = 0; i < size; i++) {
            if (i < sizeL) {
                A[i].first = left[i] & mask;
                A[i].second = left[i] >> shift;
            } else {
                A[i].first = 0;
                A[i].second = 0;
            }
            if (i < sizeR) {
                B[i].first = right[i] & mask;
                B[i].second = right[i] >> shift;
            } else {
                B[i].first = 0;
                B[i].second = 0;
            }
        }

        fft(A, C, size);
        fft(B, D, size);
        for (int i = 0; i < size; i++) {
            int j = (size - i) & sizeMask;
            pair<double, double> c0(C[i].first + C[j].first, C[i].second - C[j].second);
            pair<double, double> c1(C[i].first - C[j].first, C[i].second + C[j].second);
            pair<double, double> d0(D[i].first + D[j].first, D[i].second - D[j].second);
            pair<double, double> d1(D[i].first - D[j].first, D[i].second + D[j].second);
            //A[i] = c0 * d0 - point(0, 1) * c1 * d1;
            A[i].first = (c0.first * d0.first - c0.second * d0.second) + (c1.first * d1.second + c1.second * d1.first);
            A[i].second = (c0.first * d0.second + c0.second * d0.first) - (c1.first * d1.first - c1.second * d1.second);
            //B[i] = c0 * d1 + d0 * c1;
            B[i].first = (c0.first * d1.first - c0.second * d1.second) + (d0.first * c1.first - d0.second * c1.second);
            B[i].second = (c0.first * d1.second + c0.second * d1.first) + (d0.first * c1.second + d0.second * c1.first);
        }

        fft(A, C, size);
        fft(B, D, size);
        std::reverse(C + 1, C + size);
        std::reverse(D + 1, D + size);

        static const int SCALE2 = (1 << 2 * shift);
        static const int SCALE = (1 << shift);

        vector<int> res(sizeDst);

        int t = 4 * size;
        for (int i = 0; i < sizeDst; i++) {
            res[i] = int((llround(C[i].first / t) % mod
                        + llround(D[i].second / t) % mod * SCALE
                        + llround(C[i].second / t) % mod * SCALE2) % mod);
        }
        normalize(res);

        return res;
    }

    //--- extended operations (low order first)

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
        k = min(k, poly.size());
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

    static vector<int> square(const vector<int>& poly) {
        int n = int(poly.size());
        int sizeDst = n * 2 - 1;

        if (n < 256)
            return multiplySlow(poly, poly);

        init();

        static const int shift = 15;
        static const int mask = (1 << shift) - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;
        int sizeMask = size - 1;

        for (int i = 0; i < size; i++) {
            if (i < n) {
                A[i].first = poly[i] & mask;
                A[i].second = poly[i] >> shift;
            } else {
                A[i].first = 0;
                A[i].second = 0;
            }
        }

        fft(A, C, size);
        for (int i = 0; i < size; i++) {
            int j = (size - i) & sizeMask;
            pair<double, double> c0(C[i].first + C[j].first, C[i].second - C[j].second);
            pair<double, double> c1(C[i].first - C[j].first, C[i].second + C[j].second);
            //A[i] = c0 * c0 - point(0, 1) * c1 * c1;
            A[i].first = (c0.first * c0.first - c0.second * c0.second) + (c1.first * c1.second + c1.second * c1.first);
            A[i].second = (c0.first * c0.second + c0.second * c0.first) - (c1.first * c1.first - c1.second * c1.second);
            //B[i] = c0 * c1 + c0 * c1;
            B[i].first = (c0.first * c1.first - c0.second * c1.second) + (c0.first * c1.first - c0.second * c1.second);
            B[i].second = (c0.first * c1.second + c0.second * c1.first) + (c0.first * c1.second + c0.second * c1.first);
        }

        fft(A, C, size);
        fft(B, D, size);
        std::reverse(C + 1, C + size);
        std::reverse(D + 1, D + size);

        static const int SCALE2 = (1 << 2 * shift);
        static const int SCALE = (1 << shift);

        vector<int> res(size);

        int t = 4 * size;
        for (int i = 0; i < size; i++) {
            res[i] = int((llround(C[i].first / t) % mod
                        + llround(D[i].second / t) % mod * SCALE
                        + llround(C[i].second / t) % mod * SCALE2) % mod);
        }

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

    static vector<int> inverse(const vector<int>& a) {
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
    static vector<int> ln(const vector<int>& poly) {
        auto A = inverse(poly);
        auto B = derivate(poly);
        A = multiply(A, B);
        A.resize(poly.size());
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
            if (L == R) {
                return vector<int>{ PolyFFTMod3<mod, MaxN>::evaluate(poly, X[L]) };
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

template <int mod, int MaxN>
pair<double, double> PolyFFTMod3<mod, MaxN>::w[MaxN];

template <int mod, int MaxN>
bool PolyFFTMod3<mod, MaxN>::initialized = false;

template <int mod, int MaxN>
pair<double, double> PolyFFTMod3<mod, MaxN>::A[MaxN];

template <int mod, int MaxN>
pair<double, double> PolyFFTMod3<mod, MaxN>::B[MaxN];

template <int mod, int MaxN>
pair<double, double> PolyFFTMod3<mod, MaxN>::C[MaxN];

template <int mod, int MaxN>
pair<double, double> PolyFFTMod3<mod, MaxN>::D[MaxN];
