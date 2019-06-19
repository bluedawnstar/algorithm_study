#pragma once

/*
    Y = f(X) = a_n * X^n + a_(n-1) * X^(n - 1) + ... + a_0
        Y0 = f(X0),
        Y1 = f(X1),
         . . .
        Yn = f(Xn)

            n
    f(X) = SUM { Yj * PRODUCT (X - Xm) / (Xj - Xm) }
           j=0        0<=m=<k
                        m!=j
*/
struct LagrangePolynomial {
    // O(n^2)
    // samples = { ..., (Xi, Yi), ... }
    template <typename T, typename U>
    static double interpolate(const pair<T, U>* samples, int n, T x) {
        double res = 0;

        for (int i = 0; i < n; i++) { 
            double term = samples[i].second;
            for (int j = 0; j < n; j++) { 
                if (j != i) 
                    term *= double(x - samples[j].first) / double(samples[i].first - samples[j].first); 
            } 
            res += term; 
        } 

        return res;
    }

    // O(n^2)
    // samples = { ..., (Xi, Yi), ... }
    template <typename T, typename U>
    static double interpolate(const vector<pair<T, U>>& xy, T x) {
        return interpolate(xy.data(), int(xy.size()), x);
    }
};
