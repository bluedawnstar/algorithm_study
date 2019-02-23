#pragma once

#include "multPolyMod.h"
#include "ntt.h"

// (x + a)(x + a + 1)(x + a + 2)...(x + k)
// left = a, right = k
// High order first
inline vector<int> multPolyPolyNTT(int left, int right, int MOD, int root) {
    int n = right - left + 1;
    if (n < 128) {
        vector<int> res = vector<int>{ 1, left };
        for (int i = left + 1; i <= right; i++)
            res = multPolyMod(res, vector<int>{ 1, i }, MOD);
        return res;
    }

    vector<vector<int>> poly;
    NTT ntt(MOD, root);

    poly.push_back(vector<int>{ 1, left });
    for (int i = left + 1; i <= right; i++) {
        int j = 0;
        while (j < int(poly.size()) && !(poly[j].size() & (poly[j].size() - 1)))
            j++;

        if (j >= int(poly.size()))
            poly.push_back(vector<int>{ 1, i });
        else
            poly[j] = multPolyMod(poly[j], vector<int>{ 1, i }, MOD);

        // apply FFT
        while (j > 0 && poly[j].size() == poly[j - 1].size()
            && (poly[j].size() & (poly[j].size() - 1)) == 0) {
            if (poly[j].size() < 128)
                poly[j - 1] = multPolyMod(poly[j - 1], poly[j], MOD);
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
