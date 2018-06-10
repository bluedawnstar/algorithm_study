#pragma once

inline void init_combination(vector<int>& comb, int n, int r) {
    comb.resize(r);
    for (int i = 0; i < r; i++)
        comb[i] = i;
}

inline bool next_combination(vector<int>& comb, int n) {
    int k = (int)comb.size();
    int p = k - 1;

    if (comb[p] >= n)
        return false;

    while (p >= 0 && ++comb[p] >= n - (k - p - 1))
        --p;
    if (p < 0)
        return false;

    while (++p < k)
        comb[p] = comb[p - 1] + 1;

    return true;
}
