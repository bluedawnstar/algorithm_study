#pragma once

#include "mycomplex.h"

template <typename T = double>
bool fft(const vector<Complex<T>>& in, vector<Complex<T>>& out, bool inverse = false) {
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
            Complex<T> w(cos(M_PI * j / m), sin(M_PI * j / m));
            if (!inverse)
                w.second = -w.second;

            int step = m << 1;
            for (int i = j; i < size; i += step) {
                Complex<T> t = out[i + m] * w;
                out[i + m] = out[i] - t;
                out[i] += t;
            }
        }
    }

    if (inverse) {
        for (int i = 0; i < size; i++)
            out[i] /= size;
    }

    return true;
}

template <typename T = double>
bool fft(vector<Complex<T>>& data, bool inverse = false) {
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
            Complex<T> w(cos(M_PI * j / m), sin(M_PI * j / m));
            if (!inverse)
                w.second = -w.second;

            int step = m << 1;
            for (int i = j; i < size; i += step) {
                Complex<T> t = data[i + m] * w;
                data[i + m] = data[i] - t;
                data[i] += t;
            }
        }
    }

    if (inverse) {
        for (int i = 0; i < size; i++)
            data[i] /= size;
    }

    return true;
}
