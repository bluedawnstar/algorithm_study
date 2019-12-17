#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

struct FastFactorialIntModStandalone {
    int mod;
    int maxN;
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    FastFactorialIntModStandalone() { 
    }

    // maxN > sqrt(maxDegree) * 4
    FastFactorialIntModStandalone(int maxN, int mod) { 
        build(maxN, mod);
    }

    // maxN > sqrt(maxDegree) * 4
    void build(int maxN, int mod) {
        this->mod = mod;        
        maxN += 2;
        this->maxN = max(1, maxN);

        factorial.resize(maxN + 1);
        factInverse.resize(maxN + 1);
        inverse.resize(maxN + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxN; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxN; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            factInverse[i] = int(1ll * factInverse[i - 1] * inverse[i] % mod);
        }
    }

    //---


    // O(sqrt(N) * logN)
    int calcFactorial(int N) {
        if (N >= mod)
            return 0;
        else if (N <= maxN)
            return factorial[N];

        long long deg = 1;
        vector<int> factPart = { 1, 2 };
        while (N > deg * (deg + 1)) {
            factPart = squarePoly(factPart);
            deg *= 2;
        }

        long long res = 1;
        long long bucket = N / deg;

        for (int i = 0; i < bucket; i++)
            res = res * factPart[i] % mod;

        for (long long i = bucket * deg + 1; i <= N; i++)
            res = res * i % mod;

        return int(res);
    }


private:
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

    static const int SCALE = 32768;

    static vector<int> multiplyFT(const vector<pair<double,double>>& A1, const vector<pair<double,double>>& A2,
                                  const vector<pair<double,double>>& B1, const vector<pair<double,double>>& B2,
                                  vector<int>& result, int mod) {
        int N = int(A1.size());
        vector<pair<double,double>> C(N);

        for (int i = 0; i < N; i++) {
            // C[i] = A1[i] * B1[i]
            C[i].first = A1[i].first * B1[i].first - A1[i].second * B1[i].second;
            C[i].second = A1[i].first * B1[i].second + A1[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = int(((long long)fmod(C[i].first + 0.5, mod) * SCALE * SCALE) % mod);

        for (int i = 0; i < N; i++) {
            //C[i] = A1[i] * B2[i] + A2[i] * B1[i];
            C[i].first = A1[i].first * B2[i].first - A1[i].second * B2[i].second
                + A2[i].first * B1[i].first - A2[i].second * B1[i].second;
            C[i].second = A1[i].first * B2[i].second + A1[i].second * B2[i].first
                + A2[i].first * B1[i].second + A2[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = int((result[i] + (long long)fmod(C[i].first + 0.5, mod) * SCALE) % mod);

        for (int i = 0; i < N; i++) {
            //C[i] = A2[i] * B2[i];
            C[i].first = A2[i].first * B2[i].first - A2[i].second * B2[i].second;
            C[i].second = A2[i].first * B2[i].second + A2[i].second * B2[i].first;
        }
        fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = int((result[i] + (long long)fmod(C[i].first + 0.5, mod)) % mod);

        return result;
    }

    static vector<int> multiplySlow(const vector<int>& left, const vector<int>& right, int mod) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * left[j] * right[i]) % mod);
            }
        }

        return res;
    }

    // It's better performance than multiplySlowMod() when N >= 128
    static vector<int> multiply(const vector<int>& left, const vector<int>& right, int mod) {
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
            A1[i].first = left[i] / SCALE;
            A2[i].first = left[i] % SCALE;
        }
        for (int i = 0; i < sizeR; i++) {
            B1[i].first = right[i] / SCALE;
            B2[i].first = right[i] % SCALE;
        }

        fft(A1); fft(A2);
        fft(B1); fft(B2);

        vector<int> res(sizeDst);
        return multiplyFT(A1, A2, B1, B2, res, mod);
    }

    //--- lagrange

    /* O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<int> interpolateRange0(const int Y[], int degree, int maxX) {
        vector<int> res(maxX + 1);
        memcpy(res.data(), Y, sizeof(Y[0]) * min(degree + 1, maxX + 1));
        if (maxX <= degree)
            return res;

        vector<int> f(degree + 1);
        for (int i = 0; i <= degree; i++) {
            auto prefix = factInverse[i];
            auto suffix = factInverse[degree - i];
            if ((degree - i) & 1)
                suffix = mod - suffix;
            f[i] = int(1ll * Y[i] * prefix % mod * suffix % mod);
        }

        vector<int> conv = multiply(f, vector<int>(inverse.begin(), inverse.begin() + maxX + 1), mod);

        int g = 1;
        for (int i = 0; i <= degree; i++)
            g = int(1ll * g * (degree + 1 - i) % mod);

        res[degree + 1] = int(1ll * g * conv[degree + 1] % mod);
        for (int i = degree + 2; i <= maxX; i++) {
            g = int(1ll * g * i % mod * inverse[i - degree - 1] % mod);
            res[i] = int(1ll * g * conv[i] % mod);
        }

        return res;
    }

    /* O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<int> interpolateRange0(const vector<int>& Y, int maxX) {
        return interpolateRange0(Y.data(), int(Y.size()) - 1, maxX);
    }

    //--- factorial

    vector<int> squarePoly(const vector<int>& poly) {
        // y(0), y(1), y(2), ..., y(4 * d + 1)
        vector<int> ss = interpolateRange0(poly, (int(poly.size()) - 1) * 4 + 1);
        vector<int> res(ss.size() / 2);
        for (int i = 0; i < (int)ss.size() / 2; ++i)
            res[i] = int(1ll * ss[2 * i] * ss[2 * i + 1] % mod);
        return res;
    }
};
