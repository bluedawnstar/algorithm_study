#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
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
};
