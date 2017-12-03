#pragma once

// Assignment Problem - Hungarian Algorithm O(N^3)
template <typename T>
struct AssignmentProblem {
    static const T INF = T(0x3f3f3f3f);

    // N=rows, M=cols (N <= M)
    // Hungarian Algorithm O(N^3)
    static T doHungarian(vector<vector<T>>& A, int N, int M, vector<int>& assigned) {
        vector<T> u(N + 1);
        vector<T> v(M + 1);
        vector<int> assignRev(M + 1);
        vector<int> way(M + 1);

        for (int i = 1; i <= N; i++) {
            assignRev[0] = i;
            int j0 = 0;
            vector<T> minValueInCol(M + 1, INF);

            vector<bool> used(M + 1);
            do {
                used[j0] = true;
                int i0 = assignRev[j0];

                T delta = INF;
                int j1 = 0;
                for (int j = 1; j <= M; j++) {
                    if (!used[j]) {
                        T cur = A[i0 - 1][j - 1] - u[i0] - v[j];
                        if (cur < minValueInCol[j]) {
                            minValueInCol[j] = cur;
                            way[j] = j0;
                        }
                        if (minValueInCol[j] < delta) {
                            delta = minValueInCol[j];
                            j1 = j;
                        }
                    }
                }

                for (int j = 0; j <= M; j++) {
                    if (used[j]) {
                        u[assignRev[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minValueInCol[j] -= delta;
                    }
                }

                j0 = j1;
            } while (assignRev[j0] != 0);

            do {
                int j1 = way[j0];
                assignRev[j0] = assignRev[j1];
                j0 = j1;
            } while (j0 != 0);
        }

        assigned.assign(N, -1);
        for (int i = 1; i < (int)assignRev.size(); i++) {
            assigned[assignRev[i] - 1] = i - 1;
        }

        return -v[0];
    }

};
