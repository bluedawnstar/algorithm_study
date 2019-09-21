#pragma once

// Improved FFT
struct FFT2 {
    const double PI = acos(-1.0);

    int baseSize = 2;
    vector<pair<double,double>> baseW = {{0, 0}, {1, 0}};
    vector<int> reverse = { 0, 1 };

    // counting trailing zeros
    static int ctz(unsigned x) {
#ifndef __GNUC__
        return int(_tzcnt_u32(x));
#else
        return __builtin_ctz(x);
#endif
    }

    void initBase(int size) {
        if (size <= baseSize)
            return;

        int nbit = ctz(size);
        reverse.resize(size);
        for (int i = 0; i < size; i++)
            reverse[i] = (reverse[i >> 1] >> 1) + ((i & 1) << (nbit - 1));

        baseW.resize(size);
        while (baseSize < size) {
            double angle = PI / baseSize;
            for (int i = baseSize >> 1; i < baseSize; i++) {
                baseW[i << 1] = baseW[i];
                double angle_i = angle * (2 * i + 1 - baseSize);
                baseW[(i << 1) + 1] = make_pair(cos(angle_i), sin(angle_i));
            }
            baseSize <<= 1;
        }
    }

    bool fft(const vector<pair<double,double>>& in, vector<pair<double,double>>& out, bool inverse = false) {
        int size = int(in.size());
        if ((size & (size - 1)) != 0)
            return false;

        initBase(size);
        int shift = ctz(baseSize) - ctz(size);

        for (int i = 0; i < size; i++) {
            int j = reverse[i] >> shift;
            if (i < j) {
                out[j] = in[i];
                out[i] = in[j];
            } else if (i == j) {
                out[i] = in[i];
            }
        }

        for (int m = 1; m < size; m <<= 1) {
            for (int j = 0; j < m; j++) {
                auto w = baseW[m + j];
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

    bool fft(vector<pair<double,double>>& data, bool inverse = false) {
        int size = int(data.size());
        if ((size & (size - 1)) != 0)
            return false;

        initBase(size);
        int shift = ctz(baseSize) - ctz(size);

        for (int i = 0; i < size; i++) {
            int j = reverse[i] >> shift;
            if (i < j)
                swap(data[i], data[j]);
        }

        for (int m = 1; m < size; m <<= 1) {
            for (int j = 0; j < m; j++) {
                auto w = baseW[m + j];
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
};
