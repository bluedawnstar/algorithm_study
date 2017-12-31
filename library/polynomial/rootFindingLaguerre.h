#pragma once

#include "mycomplex.h"

#ifndef EPSILON
#define EPSILON     1.0e-9
#endif

struct LaguerreMethod {
    // Low order first
    static Complex<double> findOneRoot(const vector<Complex<double>>& poly, Complex<double> x) {
        int n = poly.size() - 1;
        
        vector<Complex<double>> p1 = derivative(poly);
        vector<Complex<double>> p2 = derivative(p1);

        for (int step = 0; step < 10000; step++) {
            Complex<double> y0 = eval(poly, x);
            if (!cmp(y0, Complex<double>(0, 0)))
                break;

            Complex<double> G = eval(p1, x) / y0;
            Complex<double> H = G * G - eval(p2, x) - y0;
            Complex<double> R = std::sqrt(std::complex<double>(Complex<double>(n - 1) * (H * Complex<double>(n) - G * G)));
            Complex<double> D1 = G + R;
            Complex<double> D2 = G - R;
            Complex<double> a = Complex<double>(n) / (cmp(D1, D2) > 0 ? D1 : D2);
            x -= a;
            if (!cmp(a, Complex<double>(0, 0)))
                break;
        }
        return x;
    }

    static vector<Complex<double>> findAllRoots(const vector<Complex<double>>& poly) {
        vector<Complex<double>> res;
        vector<Complex<double>> q = poly;
        while (q.size() > 2) {
            Complex<double> z(rand() / double(RAND_MAX), rand() / double(RAND_MAX));
            z = findOneRoot(q, z);
            z = findOneRoot(poly, z);
            q = horner(q, z).first;
            res.push_back(z);
        }
        res.push_back(-q[0] / q[1]);
        return res;
    }

private:
    // Low order first
    static pair<vector<Complex<double>>, Complex<double>> horner(const vector<Complex<double>>& poly, Complex<double> x0) {
        int N = (int)poly.size();
        vector<Complex<double>> b = vector<Complex<double>>(max(1, N - 1));

        for (int i = N - 1; i > 0; i--)
            b[i - 1] = (i < N - 1) ? poly[i] + b[i] * x0 : poly[i];
        return make_pair(b, poly[0] + b[0] * x0);
    }

    // Low order first
    static Complex<double> eval(const vector<Complex<double>>& poly, Complex<double> x) {
        return horner(poly, x).second;
    }

    // Low order first
    static vector<Complex<double>> derivative(const vector<Complex<double>>& poly) {
        int N = (int)poly.size();
        vector<Complex<double>> r = vector<Complex<double>>(max(1, N - 1));
        for (int i = 1; i < N; i++)
            r[i - 1] = poly[i] * Complex<double>(i, 0);
        return r;
    }

    static int cmp(Complex<double> x, Complex<double> y) {
        double diff = x.abs() - y.abs();
        return diff < -EPSILON ? -1 : (diff > EPSILON ? 1 : 0);
    }
};