#pragma once

/*
  Kirchhoff's theorem or Kirchhoff's matrix tree theorem (https://en.wikipedia.org/wiki/Kirchhoff%27s_theorem)
      - the number of spanning trees in a graph

  1. make Laplacian matrix of the graph
      L = D - A

    1.1) simple graphs
       D : the degree matrix of a graph
       A : the adjacency matrix of a graph (edge count)

       L = | deg(v0)     0       0      ...    0      | - |   0       A(0,1)    A(0,2)    ...  A(0,n-1) |
           |    0     deg(v1)    0      ...    0      |   | A(1,0)      0       A(0,2)    ...  A(0,n-1) |
           |    0        0     deg(v2)  ...    0      |   | A(2,0)    A(0,1)      0       ...  A(0,n-1) |
           |   ...      ...     ...     ...   ...     |   |  ...       ...       ...      ...   ...     |
           |    0        0       0      ... deg(vn-1) |   | A(n-1,0)  A(n-1,1)  A(n-1,2)  ...    0      |

    1.2) directed multigraphs
       D : a diagonal matrix with D(i,i) equal to the outdegree of vertex i
       A : a matrix with A(i,j) equal to the number of edges from i to j (including loops)

  2. calculate the number of spanning trees with KirchhoffTheoremMod::solve()
*/

struct KirchhoffTheoremMod {
    // mat : Laplacian matrix
    // O(N^3)
    static int solve(const vector<vector<int>>& mat, int mod) {
        return det(int(mat.size()) - 1, mat, mod);
    }

#if 0
    // O(N^3)
    static int det(int N, vector<vector<int>> mat, int mod) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] %= mod;

        long long res = 1;
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                for (; mat[j][i]; res = -res) {
                    long long t = mat[i][i] / mat[j][i];
                    for (int k = i; k < N; k++) {
                        mat[i][k] = (mat[i][k] - mat[j][k] * t) % mod;
                        std::swap(mat[j][k], mat[i][k]);
                    }
                }
            }
            if (mat[i][i] == 0)
                return 0;
            res = res * mat[i][i] % mod;
        }
        if (res < 0)
            res += mod;
        return static_cast<int>(res);
    }
#else
    // O(N^3)
    static int det(int N, vector<vector<int>> mat, int mod) {
        long long res = 1;
        vector<bool> used(N);

        for (int i = 0; i < N; i++) {
            int p;
            for (p = 0; p < N; p++) {
                if (!used[p] && mat[p][i])
                    break;
            }
            if (p >= N)
                return 0;

            res = (res * mat[p][i]) % mod;
            used[p] = true;

            int z = modPow(mat[p][i], mod - 2, mod);
            for (int j = 0; j < N; j++)
                mat[p][j] = static_cast<int>(1ll * mat[p][j] * z % mod);

            for (int j = 0; j < N; j++) {
                if (j != p) {
                    z = mat[j][i];
                    for (int k = 0; k < N; k++) {
                        mat[j][k] -= static_cast<int>(1ll * z * mat[p][k] % mod);
                        if (mat[j][k] >= mod)
                            mat[j][k] -= mod;
                        else if (mat[j][k] < 0)
                            mat[j][k] += mod;
                    }
                }
            }
        }
        return static_cast<int>(res);
    }

    static int modPow(int x, long long n, int mod) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return static_cast<int>(res);
    }
#endif
};
