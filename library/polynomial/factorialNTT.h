#pragma once

#include "polyNTT.h"

struct FactorialNTT {
    // High order first
    static vector<int> multiplySlow(const vector<int>& left, const vector<int>& right, int mod) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * left[j] * right[i]) % mod);
            }
        }

        return res;
    }

    // It works when M <= 998244353 (119 * 2^23 + 1, primitive root = 3)
    // (x + a)(x + a + 1)(x + a + 2)...(x + k)
    // left = a, right = k
    // High order first
    static vector<int> multiplyRisingFactorialSmall(int left, int right, int mod, int root) {
        int n = right - left + 1;
        if (n < 128) {
            vector<int> res = vector<int>{ 1, left };
            for (int i = left + 1; i <= right; i++)
                res = multiplySlow(res, vector<int>{ 1, i }, mod);
            return res;
        }

        vector<vector<int>> poly;
        NTT ntt(mod, root);

        poly.push_back(vector<int>{ 1, left });
        for (int i = left + 1; i <= right; i++) {
            int j = 0;
            while (j < int(poly.size()) && !(poly[j].size() & (poly[j].size() - 1)))
                j++;

            if (j >= int(poly.size()))
                poly.push_back(vector<int>{ 1, i });
            else
                poly[j] = multiplySlow(poly[j], vector<int>{ 1, i }, mod);

            // apply FFT
            while (j > 0 && poly[j].size() == poly[j - 1].size()
                && (poly[j].size() & (poly[j].size() - 1)) == 0) {
                if (poly[j].size() < 128)
                    poly[j - 1] = multiplySlow(poly[j - 1], poly[j], mod);
                else
                    poly[j - 1] = ntt.multiply(poly[j - 1], poly[j]);
                poly.erase(poly.begin() + j);
                j--;
            }
        }

        vector<int> res = poly.back();
        for (int i = int(poly.size()) - 2; i >= 0; i--)
            res = ntt.multiply(res, poly[i]);

        return res;
    }

    // (x + a)(x + a + 1)(x + a + 2)...(x + k)
    // left = a, right = k
    // High order first
    static vector<int> multiplyRisingFactorial(int left, int right, int mod) {
        int n = right - left + 1;
        if (n < 128) {
            vector<int> res = vector<int>{ 1, left };
            for (int i = left + 1; i <= right; i++)
                res = multiplySlow(res, vector<int>{ 1, i }, mod);
            return res;
        }

        vector<vector<int>> poly;

        poly.push_back(vector<int>{ 1, left });
        for (int i = left + 1; i <= right; i++) {
            int j = 0;
            while (j < int(poly.size()) && !(poly[j].size() & (poly[j].size() - 1)))
                j++;

            if (j >= int(poly.size()))
                poly.push_back(vector<int>{ 1, i });
            else
                poly[j] = multiplySlow(poly[j], vector<int>{ 1, i }, mod);

            // apply FFT
            while (j > 0 && poly[j].size() == poly[j - 1].size()
                && (poly[j].size() & (poly[j].size() - 1)) == 0) {
                if (poly[j].size() < 128)
                    poly[j - 1] = multiplySlow(poly[j - 1], poly[j], mod);
                else
                    poly[j - 1] = PolyNTT::multiplyFast(poly[j - 1], poly[j], mod);
                poly.erase(poly.begin() + j);
                j--;
            }
        }

        vector<int> res = poly.back();
        for (int i = int(poly.size()) - 2; i >= 0; i--)
            res = PolyNTT::multiplyFast(res, poly[i], mod);

        return res;
    }
};
