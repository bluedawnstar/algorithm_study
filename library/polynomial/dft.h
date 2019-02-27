#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

struct DFT {
    static void dft(const vector<pair<double,double>>& in, vector<pair<double,double>>& out, bool inverse = false) {
        int size = int(in.size());
        double wk, wn = 2 * M_PI / size;

        if (inverse)
            wn = -wn;

        for (int k = 0; k < size; k++) {
            out[k].first = 0.0;
            out[k].second = 0.0;

            wk = k * wn;
            for (int n = 0; n < size; n++) {
                double c = cos(n * wk), s = sin(n * wk);
                out[k].first += in[n].first *  c + in[n].second * s;
                out[k].second += in[n].first * -s + in[n].second * c;
            }

            if (inverse) {
                out[k].first /= size;
                out[k].second /= size;
            }
        }
    }
};
