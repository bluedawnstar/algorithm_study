#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

// 1) N, mod < 2^45(=35,184,372,088,832)
//    struct FastFactorialIntModStandaloneLL45
// 
// 2) N, mod < 2^34(=17,179,869,184)
//    struct FastFactorialIntModStandaloneLL34

// https://www.acmicpc.net/problem/17468

// N, mod < 2^45
struct FastFactorialIntModStandaloneLL45 {
    int maxN;
    long long mod;
    vector<long long> factorial;    // factorial
    vector<long long> factInverse;  // inverse factorial
    vector<long long> inverse;

    FastFactorialIntModStandaloneLL45() { 
    }

    // maxN > sqrt(maxDegree) * 4
    FastFactorialIntModStandaloneLL45(int maxN, long long mod) { 
        build(maxN, mod);
    }

    // maxN > sqrt(maxDegree) * 4
    void build(int maxN, long long mod) {
        maxN += 2;
        this->maxN = max(1, maxN);
        this->mod = mod;        

        factorial.resize(maxN + 1);
        factInverse.resize(maxN + 1);
        inverse.resize(maxN + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxN; i++) {
            inverse[i] = mod - modMul(mod / i, inverse[mod % i], mod);
            if (inverse[i] >= mod)
                inverse[i] -= mod;
        }

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxN; i++) {
            factorial[i] = modMul(factorial[i - 1], i, mod);
            factInverse[i] = modMul(factInverse[i - 1], inverse[i], mod);
        }
    }

    //---

    // O(sqrt(N) * logN)
    long long calcFactorial(long long N) {
        if (N >= mod)
            return 0ll;
        else if (N <= maxN)
            return factorial[N];

        long long deg = 1;
        vector<long long> factPart = { 1, 2 };
        while (N > deg * (deg + 1)) {
            factPart = squarePoly(factPart);
            deg *= 2;
        }

        long long res = 1;
        long long bucket = N / deg;

        for (int i = 0; i < bucket; i++)
            res = modMul(res, factPart[i], mod);

        for (long long i = bucket * deg + 1; i <= N; i++)
            res = modMul(res, i, mod);

        return res;
    }


private:
    //*///---
    static long long modMul(long long a, long long b, long long mod) {
#ifdef __GNUC__
        return static_cast<long long>((__int128_t)(a % mod) * (b % mod) % mod);
#else
        int base = int(1e9);
        long long aLow = a % base, aHigh = a / base;
        long long bLow = b % base, bHigh = b / base;

        long long result = (aHigh * bHigh) % mod;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % mod;

        result = (result + aLow * bHigh % mod + bLow * aHigh % mod) % mod;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % mod;

        result = (result + aLow * bLow % mod) % mod;

        return result;
#endif
    }
    /*///---
    static long long modMul(long long a, long long b, long long mod) {
#ifdef __GNUC__
        return static_cast<long long>((__int128_t)(a % mod) * (b % mod) % mod);
#else
        if (b == 0)
            return 0;

        long long res = modMul(a, b / 2, mod);
        res = (res + res) % mod;

        return ((b & 1) == 0) ? res : (res + a) % mod;
#endif
    }
    //*///---

    static long long modPow(long long x, long long n, long long mod) {
        n %= mod - 1;

        if (n == 0)
            return 1ll;

        long long res = 1ll;
#ifdef __GNUC__
        long long t = x % mod;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = static_cast<long long>((__int128_t)t * res % mod);
            t = static_cast<long long>((__int128_t)t * t % mod);
        }
#else
        long long t = x % mod;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = modMul(res, t, mod);
            t = modMul(t, t, mod);
        }
#endif
        return res;
    }

    static long long modInv(long long a, long long mod) {
        return modPow(a, mod - 2, mod);
    }

    //--- FFT

    static bool fft(const vector<pair<double,double>>& in, vector<pair<double,double>>& out, bool inverse = false) {
        int size = int(in.size());
        if ((size & (size - 1)) != 0)
            return false;

        for (int i = 0, j = 0; i < size; i++) {
            out[j] = in[i];

            int mask = size;
            while (j & (mask >>= 1))
                j &= ~mask;
            j |= mask;
        }

        for (int m = 1; m < size; m <<= 1) {
            for (int j = 0; j < m; j++) {
                pair<double,double> w(cos(M_PI * j / m), sin(M_PI * j / m));
                if (!inverse)
                    w.second = -w.second;

                int step = m << 1;
                for (int i = j; i < size; i += step) {
                    pair<double, double> t(out[i + m].first * w.first - out[i + m].second * w.second,
                                           out[i + m].first * w.second + out[i + m].second * w.first);
                    out[i + m].first = out[i].first - t.first;
                    out[i + m].second = out[i].second - t.second;
                    out[i].first += t.first;
                    out[i].second += t.second;
                }
            }
        }

        if (inverse) {
            for (int i = 0; i < size; i++) {
                out[i].first /= size;
                out[i].second /= size;
            }
        }

        return true;
    }

    static bool fft(vector<pair<double,double>>& data, bool inverse = false) {
        int size = int(data.size());
        if ((size & (size - 1)) != 0)
            return false;

        for (int i = 0, j = 0; i < size; i++) {
            if (j > i)
                swap(data[i], data[j]);

            int mask = size;
            while (j & (mask >>= 1))
                j &= ~mask;
            j |= mask;
        }

        for (int m = 1; m < size; m <<= 1) {
            for (int j = 0; j < m; j++) {
                pair<double,double> w(cos(M_PI * j / m), sin(M_PI * j / m));
                if (!inverse)
                    w.second = -w.second;

                int step = m << 1;
                for (int i = j; i < size; i += step) {
                    pair<double, double> t(data[i + m].first * w.first - data[i + m].second * w.second,
                                           data[i + m].first * w.second + data[i + m].second * w.first);
                    data[i + m].first = data[i].first - t.first;
                    data[i + m].second = data[i].second - t.second;
                    data[i].first += t.first;
                    data[i].second += t.second;
                }
            }
        }

        if (inverse) {
            for (int i = 0; i < size; i++) {
                data[i].first /= size;
                data[i].second /= size;
            }
        }

        return true;
    }

    //--- polynomial multiplication by FFT

    static const long long SCALE1 = (1ll << 15);
    static const long long SCALE2 = (1ll << 30);
    static const long long SCALE3 = (1ll << 45);
    static const long long SCALE4 = (1ll << 60);

    static vector<long long> multiplyFT(
            const vector<pair<double,double>>& A1, const vector<pair<double,double>>& A2, const vector<pair<double,double>>& A3, 
            const vector<pair<double,double>>& B1, const vector<pair<double,double>>& B2, const vector<pair<double,double>>& B3,
            vector<long long>& result, long long mod) {
        int N = int(A1.size());
        vector<pair<double,double>> C(N);

        for (int i = 0; i < N; i++) {
            // C[i] = A1[i] * B1[i]
            C[i].first = A1[i].first * B1[i].first - A1[i].second * B1[i].second;
            C[i].second = A1[i].first * B1[i].second + A1[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = modMul(static_cast<long long>(fmod(C[i].first + 0.5, mod)), SCALE4 % mod, mod);

        for (int i = 0; i < N; i++) {
            //C[i] = A1[i] * B2[i] + A2[i] * B1[i];
            C[i].first = A1[i].first * B2[i].first - A1[i].second * B2[i].second
                       + A2[i].first * B1[i].first - A2[i].second * B1[i].second;
            C[i].second = A1[i].first * B2[i].second + A1[i].second * B2[i].first
                        + A2[i].first * B1[i].second + A2[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++) {
            result[i] += modMul(static_cast<long long>(fmod(C[i].first + 0.5, mod)), SCALE3 % mod, mod);
            if (result[i] >= mod)
                result[i] -= mod;
        }

        for (int i = 0; i < N; i++) {
            //C[i] = A3[i] * B1[i] + A2[i] * B2[i] + A1[i] * B3[i];
            C[i].first = A3[i].first * B1[i].first - A3[i].second * B1[i].second
                       + A2[i].first * B2[i].first - A2[i].second * B2[i].second
                       + A1[i].first * B3[i].first - A1[i].second * B3[i].second;
            C[i].second = A3[i].first * B1[i].second + A3[i].second * B1[i].first
                        + A2[i].first * B2[i].second + A2[i].second * B2[i].first
                        + A1[i].first * B3[i].second + A1[i].second * B3[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++) {
            result[i] += modMul(static_cast<long long>(fmod(C[i].first + 0.5, mod)), SCALE2 % mod, mod);
            if (result[i] >= mod)
                result[i] -= mod;
        }

        for (int i = 0; i < N; i++) {
            //C[i] = A2[i] * B3[i] + A3[i] * B2[i];
            C[i].first = A2[i].first * B3[i].first - A2[i].second * B3[i].second
                       + A3[i].first * B2[i].first - A3[i].second * B2[i].second;
            C[i].second = A2[i].first * B3[i].second + A2[i].second * B3[i].first
                        + A3[i].first * B2[i].second + A3[i].second * B2[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++) {
            result[i] += modMul(static_cast<long long>(fmod(C[i].first + 0.5, mod)), SCALE1 % mod, mod);
            if (result[i] >= mod)
                result[i] -= mod;
        }

        for (int i = 0; i < N; i++) {
            //C[i] = A3[i] * B3[i];
            C[i].first  = A3[i].first * B3[i].first - A3[i].second * B3[i].second;
            C[i].second = A3[i].first * B3[i].second + A3[i].second * B3[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++) {
            result[i] += static_cast<long long>(fmod(C[i].first + 0.5, mod));
            if (result[i] >= mod)
                result[i] -= mod;
        }

        return result;
    }

    static vector<long long> multiplySlow(const vector<long long>& left, const vector<long long>& right, long long mod) {
        vector<long long> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] += modMul(left[j], right[i], mod);
                if (res[i + j] >= mod)
                    res[i + j] -= mod;
            }
        }

        return res;
    }

    // It's better performance than multiplySlowMod() when N >= 128
    static vector<long long> multiply(const vector<long long>& left, const vector<long long>& right, long long mod) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

        if (sizeDst <= 256)
            return multiplySlow(left, right, mod);

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A1(size), A2(size), A3(size);
        vector<pair<double,double>> B1(size), B2(size), B3(size);
        for (int i = 0; i < sizeL; i++) {
            A1[i].first = static_cast<double>((left[i] >> 30)               );
            A2[i].first = static_cast<double>((left[i] >> 15) & (SCALE1 - 1));
            A3[i].first = static_cast<double>((left[i]      ) & (SCALE1 - 1));
        }
        for (int i = 0; i < sizeR; i++) {
            B1[i].first = static_cast<double>((right[i] >> 30)               );
            B2[i].first = static_cast<double>((right[i] >> 15) & (SCALE1 - 1));
            B3[i].first = static_cast<double>((right[i]      ) & (SCALE1 - 1));
        }

        fft(A1); fft(A2); fft(A3);
        fft(B1); fft(B2); fft(B3);

        vector<long long> res(sizeDst);
        return multiplyFT(A1, A2, A3, B1, B2, B3, res, mod);
    }

    //--- lagrange

    /* multipoint evaluation - O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<long long> interpolateRange0(const long long Y[], int degree, int maxX) {
        vector<long long> res(maxX + 1);
        memcpy(res.data(), Y, sizeof(Y[0]) * min(degree + 1, maxX + 1));
        if (maxX <= degree)
            return res;

        vector<long long> f(degree + 1);
        for (int i = 0; i <= degree; i++) {
            auto prefix = factInverse[i];
            auto suffix = factInverse[degree - i];
            if ((degree - i) & 1)
                suffix = mod - suffix;
            f[i] = modMul(Y[i], modMul(prefix, suffix, mod), mod);
        }

        vector<long long> conv = multiply(f, vector<long long>(inverse.begin(), inverse.begin() + maxX + 1), mod);

        long long g = 1;
        for (int i = 0; i <= degree; i++)
            g = modMul(g, degree + 1 - i, mod);

        res[degree + 1] = modMul(g, conv[degree + 1], mod);
        for (int i = degree + 2; i <= maxX; i++) {
            g = modMul(g, modMul(i, inverse[i - degree - 1], mod), mod);
            res[i] = modMul(g, conv[i], mod);
        }

        return res;
    }

    /* multipoint evaluation - O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<long long> interpolateRange0(const vector<long long>& Y, int maxX) {
        return interpolateRange0(Y.data(), int(Y.size()) - 1, maxX);
    }

    //--- factorial

    vector<long long> squarePoly(const vector<long long>& poly) {
        // y(0), y(1), y(2), ..., y(4 * d + 1)
        vector<long long> ss = interpolateRange0(poly, (int(poly.size()) - 1) * 4 + 1);
        vector<long long> res(ss.size() / 2);
        for (int i = 0; i < (int)ss.size() / 2; ++i)
            res[i] = modMul(ss[2 * i], ss[2 * i + 1], mod);
        return res;
    }
};

// N, mod < 2^34
struct FastFactorialIntModStandaloneLL34 {
    int maxN;
    long long mod;
    vector<long long> factorial;    // factorial
    vector<long long> factInverse;  // inverse factorial
    vector<long long> inverse;

    FastFactorialIntModStandaloneLL34() { 
    }

    // maxN > sqrt(maxDegree) * 4
    FastFactorialIntModStandaloneLL34(int maxN, long long mod) { 
        build(maxN, mod);
    }

    // maxN > sqrt(maxDegree) * 4
    void build(int maxN, long long mod) {
        maxN += 2;
        this->maxN = max(1, maxN);
        this->mod = mod;        

        factorial.resize(maxN + 1);
        factInverse.resize(maxN + 1);
        inverse.resize(maxN + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxN; i++) {
            inverse[i] = mod - modMul(mod / i, inverse[mod % i], mod);
            if (inverse[i] >= mod)
                inverse[i] -= mod;
        }

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxN; i++) {
            factorial[i] = modMul(factorial[i - 1], i, mod);
            factInverse[i] = modMul(factInverse[i - 1], inverse[i], mod);
        }
    }

    //---

    // O(sqrt(N) * logN)
    long long calcFactorial(long long N) {
        if (N >= mod)
            return 0ll;
        else if (N <= maxN)
            return factorial[N];

        long long deg = 1;
        vector<long long> factPart = { 1, 2 };
        while (N > deg * (deg + 1)) {
            factPart = squarePoly(factPart);
            deg *= 2;
        }

        long long res = 1;
        long long bucket = N / deg;

        for (int i = 0; i < bucket; i++)
            res = modMul(res, factPart[i], mod);

        for (long long i = bucket * deg + 1; i <= N; i++)
            res = modMul(res, i, mod);

        return res;
    }


private:
    //*///---
    static long long modMul(long long a, long long b, long long mod) {
#ifdef __GNUC__
        return static_cast<long long>((__int128_t)(a % mod) * (b % mod) % mod);
#else
        int base = int(1e9);
        long long aLow = a % base, aHigh = a / base;
        long long bLow = b % base, bHigh = b / base;

        long long result = (aHigh * bHigh) % mod;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % mod;

        result = (result + aLow * bHigh % mod + bLow * aHigh % mod) % mod;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % mod;

        result = (result + aLow * bLow % mod) % mod;

        return result;
#endif
    }
    /*///---
    static long long modMul(long long a, long long b, long long mod) {
#ifdef __GNUC__
        return static_cast<long long>((__int128_t)(a % mod) * (b % mod) % mod);
#else
        if (b == 0)
            return 0;

        long long res = modMul(a, b / 2, mod);
        res = (res + res) % mod;

        return ((b & 1) == 0) ? res : (res + a) % mod;
#endif
    }
    //*///---

    static long long modPow(long long x, long long n, long long mod) {
        n %= mod - 1;

        if (n == 0)
            return 1ll;

        long long res = 1ll;
#ifdef __GNUC__
        long long t = x % mod;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = static_cast<long long>((__int128_t)t * res % mod);
            t = static_cast<long long>((__int128_t)t * t % mod);
        }
#else
        long long t = x % mod;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = modMul(res, t, mod);
            t = modMul(t, t, mod);
        }
#endif
        return res;
    }

    static long long modInv(long long a, long long mod) {
        return modPow(a, mod - 2, mod);
    }

    //--- FFT

    static bool fft(const vector<pair<double,double>>& in, vector<pair<double,double>>& out, bool inverse = false) {
        int size = int(in.size());
        if ((size & (size - 1)) != 0)
            return false;

        for (int i = 0, j = 0; i < size; i++) {
            out[j] = in[i];

            int mask = size;
            while (j & (mask >>= 1))
                j &= ~mask;
            j |= mask;
        }

        for (int m = 1; m < size; m <<= 1) {
            for (int j = 0; j < m; j++) {
                pair<double,double> w(cos(M_PI * j / m), sin(M_PI * j / m));
                if (!inverse)
                    w.second = -w.second;

                int step = m << 1;
                for (int i = j; i < size; i += step) {
                    pair<double, double> t(out[i + m].first * w.first - out[i + m].second * w.second,
                                           out[i + m].first * w.second + out[i + m].second * w.first);
                    out[i + m].first = out[i].first - t.first;
                    out[i + m].second = out[i].second - t.second;
                    out[i].first += t.first;
                    out[i].second += t.second;
                }
            }
        }

        if (inverse) {
            for (int i = 0; i < size; i++) {
                out[i].first /= size;
                out[i].second /= size;
            }
        }

        return true;
    }

    static bool fft(vector<pair<double,double>>& data, bool inverse = false) {
        int size = int(data.size());
        if ((size & (size - 1)) != 0)
            return false;

        for (int i = 0, j = 0; i < size; i++) {
            if (j > i)
                swap(data[i], data[j]);

            int mask = size;
            while (j & (mask >>= 1))
                j &= ~mask;
            j |= mask;
        }

        for (int m = 1; m < size; m <<= 1) {
            for (int j = 0; j < m; j++) {
                pair<double,double> w(cos(M_PI * j / m), sin(M_PI * j / m));
                if (!inverse)
                    w.second = -w.second;

                int step = m << 1;
                for (int i = j; i < size; i += step) {
                    pair<double, double> t(data[i + m].first * w.first - data[i + m].second * w.second,
                                           data[i + m].first * w.second + data[i + m].second * w.first);
                    data[i + m].first = data[i].first - t.first;
                    data[i + m].second = data[i].second - t.second;
                    data[i].first += t.first;
                    data[i].second += t.second;
                }
            }
        }

        if (inverse) {
            for (int i = 0; i < size; i++) {
                data[i].first /= size;
                data[i].second /= size;
            }
        }

        return true;
    }

    //--- polynomial multiplication by FFT

    static const long long SCALE = (1ll << 17);
    static const long long SCALE2 = (1ll << 34);

    static vector<long long> multiplyFT(
            const vector<pair<double,double>>& A1, const vector<pair<double,double>>& A2,
            const vector<pair<double,double>>& B1, const vector<pair<double,double>>& B2,
            vector<long long>& result, long long mod) {
        int N = int(A1.size());
        vector<pair<double,double>> C(N);

        for (int i = 0; i < N; i++) {
            // C[i] = A1[i] * B1[i]
            C[i].first = A1[i].first * B1[i].first - A1[i].second * B1[i].second;
            C[i].second = A1[i].first * B1[i].second + A1[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = modMul(static_cast<long long>(fmod(C[i].first + 0.5, mod)), SCALE2 % mod, mod);

        for (int i = 0; i < N; i++) {
            //C[i] = A1[i] * B2[i] + A2[i] * B1[i];
            C[i].first = A1[i].first * B2[i].first - A1[i].second * B2[i].second
                       + A2[i].first * B1[i].first - A2[i].second * B1[i].second;
            C[i].second = A1[i].first * B2[i].second + A1[i].second * B2[i].first
                        + A2[i].first * B1[i].second + A2[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++) {
            result[i] += modMul(static_cast<long long>(fmod(C[i].first + 0.5, mod)), SCALE % mod, mod);
            if (result[i] >= mod)
                result[i] -= mod;
        }

        for (int i = 0; i < N; i++) {
            //C[i] = A2[i] * B2[i];
            C[i].first = A2[i].first * B2[i].first - A2[i].second * B2[i].second;
            C[i].second = A2[i].first * B2[i].second + A2[i].second * B2[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++) {
            result[i] += static_cast<long long>(fmod(C[i].first + 0.5, mod));
            if (result[i] >= mod)
                result[i] -= mod;
        }

        return result;
    }

    static vector<long long> multiplySlow(const vector<long long>& left, const vector<long long>& right, long long mod) {
        vector<long long> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] += modMul(left[j], right[i], mod);
                if (res[i + j] >= mod)
                    res[i + j] -= mod;
            }
        }

        return res;
    }

    // It's better performance than multiplySlowMod() when N >= 128
    static vector<long long> multiply(const vector<long long>& left, const vector<long long>& right, long long mod) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

        if (sizeDst <= 256)
            return multiplySlow(left, right, mod);

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A1(size), A2(size);
        vector<pair<double,double>> B1(size), B2(size);
        for (int i = 0; i < sizeL; i++) {
            A1[i].first = static_cast<double>(left[i] / SCALE);
            A2[i].first = static_cast<double>(left[i] % SCALE);
        }
        for (int i = 0; i < sizeR; i++) {
            B1[i].first = static_cast<double>(right[i] / SCALE);
            B2[i].first = static_cast<double>(right[i] % SCALE);
        }

        fft(A1); fft(A2);
        fft(B1); fft(B2);

        vector<long long> res(sizeDst);
        return multiplyFT(A1, A2, B1, B2, res, mod);
    }

    //--- lagrange

    /* multipoint evaluation - O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<long long> interpolateRange0(const long long Y[], int degree, int maxX) {
        vector<long long> res(maxX + 1);
        memcpy(res.data(), Y, sizeof(Y[0]) * min(degree + 1, maxX + 1));
        if (maxX <= degree)
            return res;

        vector<long long> f(degree + 1);
        for (int i = 0; i <= degree; i++) {
            auto prefix = factInverse[i];
            auto suffix = factInverse[degree - i];
            if ((degree - i) & 1)
                suffix = mod - suffix;
            f[i] = modMul(Y[i], modMul(prefix, suffix, mod), mod);
        }

        vector<long long> conv = multiply(f, vector<long long>(inverse.begin(), inverse.begin() + maxX + 1), mod);

        long long g = 1;
        for (int i = 0; i <= degree; i++)
            g = modMul(g, degree + 1 - i, mod);

        res[degree + 1] = modMul(g, conv[degree + 1], mod);
        for (int i = degree + 2; i <= maxX; i++) {
            g = modMul(g, modMul(i, inverse[i - degree - 1], mod), mod);
            res[i] = modMul(g, conv[i], mod);
        }

        return res;
    }

    /* multipoint evaluation - O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<long long> interpolateRange0(const vector<long long>& Y, int maxX) {
        return interpolateRange0(Y.data(), int(Y.size()) - 1, maxX);
    }

    //--- factorial

    vector<long long> squarePoly(const vector<long long>& poly) {
        // y(0), y(1), y(2), ..., y(4 * d + 1)
        vector<long long> ss = interpolateRange0(poly, (int(poly.size()) - 1) * 4 + 1);
        vector<long long> res(ss.size() / 2);
        for (int i = 0; i < (int)ss.size() / 2; ++i)
            res[i] = modMul(ss[2 * i], ss[2 * i + 1], mod);
        return res;
    }
};
