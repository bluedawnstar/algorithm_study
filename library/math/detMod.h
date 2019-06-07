#pragma once


// Determinant modulo with gaussian elimination method
// N is the determinant size, calculate |mat| % MOD
// 
// O(N^3*logN)
template <typename T>
struct DetMod {
    static T det(int N, vector<vector<T>> mat, int MOD) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                mat[i][j] %= MOD;
            }
        }

        long long res = 1;
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                for (; mat[j][i]; res = -res) {
                    long long t = mat[i][i] / mat[j][i];
                    for (int k = i; k < N; k++) {
                        mat[i][k] = (mat[i][k] - mat[j][k] * t) % MOD;
                        std::swap(mat[j][k], mat[i][k]);
                    }
                }
            }
            if (mat[i][i] == 0)
                return 0;
            res = res * mat[i][i] % MOD;
        }
        if (res < 0)
            res += MOD;

        return T(res);
    }
};
