#pragma once

#include "polyFFTMod2.h"

template <int mod>
struct FactorialFFTMod2 : public polyFFTMod2 {
    // rising factorial, Pochhammer function, Pochhammer polynomial, ascending factorial, rising sequential product, or upper factorial
    // (x + a)(x + a + 1)(x + a + 2)...(x + k)
    // left = a, right = k, High order first
    vector<int> multiplyRisingFactorial(int left, int right) {
        int n = right - left + 1;
        if (n < 128) {
            vector<int> res = vector<int>{ 1, left };
            for (int i = left + 1; i <= right; i++)
                res = multiplySlow(res, vector<int>{ 1, i });
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
                poly[j] = multiplySlow(poly[j], vector<int>{ 1, i });

            // apply FFT
            while (j > 0 && poly[j].size() == poly[j - 1].size()
                && (poly[j].size() & (poly[j].size() - 1)) == 0) {
                if (poly[j].size() < 128)
                    poly[j - 1] = multiplySlow(poly[j - 1], poly[j]);
                else
                    poly[j - 1] = multiply(poly[j - 1], poly[j]);
                poly.erase(poly.begin() + j);
                j--;
            }
        }

        vector<int> res = poly.back();
        for (int i = int(poly.size()) - 2; i >= 0; i--)
            res = multiply(res, poly[i]);

        return res;
    }
};
