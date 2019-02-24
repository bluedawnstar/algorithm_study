#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

#ifndef SCALE
#define SCALE   32768
#endif

struct FFT {
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
                    out[i + m].second = out[i].first - t.second;
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
                    data[i + m].second = data[i].first - t.second;
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

    //--- polynomial multiplication

    // High order first
    static vector<int> multiplySlow(const vector<int>& left, const vector<int>& right) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] += left[j] * right[i];
            }
        }

        return res;
    }

    // It's better performance than multiplySlow() when N >= 256
    // High order first
    static vector<int> multiply(const vector<int>& left, const vector<int>& right) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A(size);
        vector<pair<double,double>> B(size);
        for (int i = 0; i < sizeL; i++)
            A[i].first = left[i];
        for (int i = 0; i < sizeR; i++)
            B[i].first = right[i];

        fft(A);
        fft(B);

        vector<pair<double,double>> C(size);
        for (int i = 0; i < size; i++) {
            //C[i] = A[i] * B[i];
            C[i].first = A[i].first * B[i].first - A[i].second * B[i].second;
            C[i].second = A[i].first * B[i].second + A[i].second * B[i].first;
        }

        fft(C, true);

        vector<int> res(sizeDst);
        for (int i = 0; i < sizeDst; i++)
            res[i] = int(C[i].first + 0.5);

        return res;
    }

    //--- polynomial modular multiplication

    // High order first
    static vector<int> multiplySlowMod(const vector<int>& left, const vector<int>& right, int MOD) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * left[j] * right[i]) % MOD);
            }
        }

        return res;
    }

    // It's better performance than multiplySlowMod() when N >= 128
    // High order first
    static vector<int> multiplyMod(const vector<int>& left, const vector<int>& right, int MOD) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

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
        vector<pair<double,double>> C(size);

        for (int i = 0; i < size; i++) {
            C[i].first = A1[i].first * B1[i].first - A1[i].second * B1[i].second;
            C[i].second = A1[i].first * B1[i].second + A1[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < sizeDst; i++)
            res[i] = int(((long long)fmod(C[i].first + 0.5, MOD) * SCALE * SCALE) % MOD);

        for (int i = 0; i < size; i++) {
            C[i].first = A1[i].first * B2[i].first - A1[i].second * B2[i].second
                       + A2[i].first * B1[i].first - A2[i].second * B1[i].second;
            C[i].second = A1[i].first * B2[i].second + A1[i].second * B2[i].first
                        + A2[i].first * B1[i].second + A2[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < sizeDst; i++)
            res[i] = int((res[i] + (long long)fmod(C[i].first + 0.5, MOD) * SCALE) % MOD);

        for (int i = 0; i < size; i++) {
            C[i].first = A2[i].first * B2[i].first - A2[i].second * B2[i].second;
            C[i].second = A2[i].first * B2[i].second + A2[i].second * B2[i].first;
        }
        fft(C, true);
        for (int i = 0; i < sizeDst; i++)
            res[i] = int((res[i] + (long long)fmod(C[i].first + 0.5, MOD)) % MOD);

        return res;
    }

    // rising factorial, Pochhammer function, Pochhammer polynomial, ascending factorial, rising sequential product, or upper factorial
    // (x + a)(x + a + 1)(x + a + 2)...(x + k)
    // left = a, right = k, High order first
    static vector<int> multiplyRisingFactorialMod(int left, int right, int MOD) {
        int n = right - left + 1;
        if (n < 128) {
            vector<int> res = vector<int>{ 1, left };
            for (int i = left + 1; i <= right; i++)
                res = multiplySlowMod(res, vector<int>{ 1, i }, MOD);
            return res;
        }

        vector<vector<int>> poly;

        poly.push_back(vector<int>{ 1, left });
        for (int i = left + 1; i <= right; i++) {
            int j = 0;
            while (j < int(poly.size()) && !(poly[j].size() & (poly[j].size() - 1)))
                j++;

            // make the number of polynomial terms 2^n
            if (j >= int(poly.size()))
                poly.push_back(vector<int>{ 1, i });
            else
                poly[j] = multiplySlowMod(poly[j], vector<int>{ 1, i }, MOD);

            // apply FFT
            while (j > 0 && poly[j].size() == poly[j - 1].size()
                && (poly[j].size() & (poly[j].size() - 1)) == 0) {
                if (poly[j].size() < 128)
                    poly[j - 1] = multiplySlowMod(poly[j - 1], poly[j], MOD);
                else
                    poly[j - 1] = multiplyMod(poly[j - 1], poly[j], MOD);
                poly.erase(poly.begin() + j);
                j--;
            }
        }

        vector<int> res = poly.back();
        for (int i = int(poly.size()) - 2; i >= 0; i--)
            res = multiplyMod(res, poly[i], MOD);

        return res;
    }
};
