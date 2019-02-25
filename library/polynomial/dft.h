#pragma once

#include "mycomplex.h"

template <typename T = double>
void dft(const vector<Complex<T>>& in, vector<Complex<T>>& out, bool inverse = false) {
    int size = int(in.size());
    T wk, wn = 2 * M_PI / size;

    if (inverse)
        wn = -wn;

    for (int k = 0; k < size; k++) {
        out[k].set(0, 0);

        wk = k * wn;
        for (int n = 0; n < size; n++) {
            double c = cos(n * wk), s = sin(n * wk);
            out[k].first += in[n].first *  c + in[n].second * s;
            out[k].second += in[n].first * -s + in[n].second * c;
        }

        if (inverse)
            out[k] /= size;
    }
}
