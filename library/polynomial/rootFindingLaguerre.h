#pragma once

#include "mycomplex.h"

#ifndef EPSILON
#define EPSILON     1.0e-9
#endif

struct LaguerreMethod {
    // Low order first
    static Complex findOneRoot(const vector<Complex>& poly, Complex x) {
        int n = int(poly.size()) - 1;
        
        vector<Complex> p1 = derivative(poly);
        vector<Complex> p2 = derivative(p1);

        for (int step = 0; step < 10000; step++) {
            Complex y0 = eval(poly, x);
            if (!cmp(y0, Complex(0, 0)))
                break;

            Complex G = eval(p1, x) / y0;
            Complex H = G * G - eval(p2, x) - y0;
            Complex R = std::sqrt(std::complex<double>(Complex(n - 1) * (H * Complex(n) - G * G)));
            Complex D1 = G + R;
            Complex D2 = G - R;
            Complex a = Complex(n) / (cmp(D1, D2) > 0 ? D1 : D2);
            x -= a;
            if (!cmp(a, Complex(0, 0)))
                break;
        }
        return x;
    }

    static vector<Complex> findAllRoots(const vector<Complex>& poly) {
        vector<Complex> res;
        vector<Complex> q = poly;
        while (q.size() > 2) {
            Complex z(rand() / double(RAND_MAX), rand() / double(RAND_MAX));
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
    static pair<vector<Complex>, Complex> horner(const vector<Complex>& poly, Complex x0) {
        int N = int(poly.size());
        vector<Complex> b = vector<Complex>(max(1, N - 1));

        for (int i = N - 1; i > 0; i--)
            b[i - 1] = (i < N - 1) ? poly[i] + b[i] * x0 : poly[i];
        return make_pair(b, poly[0] + b[0] * x0);
    }

    // Low order first
    static Complex eval(const vector<Complex>& poly, Complex x) {
        return horner(poly, x).second;
    }

    // Low order first
    static vector<Complex> derivative(const vector<Complex>& poly) {
        int N = int(poly.size());
        vector<Complex> r = vector<Complex>(max(1, N - 1));
        for (int i = 1; i < N; i++)
            r[i - 1] = poly[i] * Complex(i, 0);
        return r;
    }

    static int cmp(Complex x, Complex y) {
        double diff = x.abs() - y.abs();
        return diff < -EPSILON ? -1 : (diff > EPSILON ? 1 : 0);
    }
};