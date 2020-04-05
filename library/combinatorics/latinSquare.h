#pragma once

// Indicium in Qualification Round 2020 - Code Jam 2020

// 1 <= latinSquare[row][col] <= N
struct LatinSquare {
    // K = the trace of a square matrix is the sum of the values on the main diagonal
    static bool findAnyWithTrace(vector<vector<int>>& out, int N, int K) {
        vector<int> diag = findDiag(N, K);
        if (diag.empty())
            return false;
        return findAnyWithDiag(out, N, diag);
    }

    // diag = main diag
    static bool findAnyWithDiag(vector<vector<int>>& out, int N, const vector<int>& diag) {
        vector<bool> S(N + 1);
        for (int d : diag)
            S[d] = true;

        out = vector<vector<int>>(N, vector<int>(N, -1));
        for (int i = 0; i < N; i++)
            out[i][i] = diag[i];

        // step #1 : numbers in diagonal
        for (int a = 1; a <= N; a++) {
            if (!S[a])
                continue;

            vector<vector<int>> m(N, vector<int>(N, 0));
            for (int i = 0; i < N; i++) {
                int found = -1;
                for (int j = 0; j < N; j++) {
                    if (out[i][j] == a)
                        found = j;
                }
                if (found >= 0) {
                    m[i][found] = 1;
                } else {
                    for (int j = 0; j < N; j++) {
                        if (out[i][j] < 0)
                            m[i][j] = 1;
                    }
                }
            }
            vector<int> lhs, rhs;
            if (bipartiteMatching(m, lhs, rhs) != N)
                return false;
            for (int i = 0; i < N; i++)
                out[i][lhs[i]] = a;
        }

        // step #2 : numbers not in diagonal
        for (int a = 1; a <= N; a++) {
            if (S[a])
                continue;

            vector<vector<int>> m(N, vector<int>(N, 0));
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (out[i][j] < 0)
                        m[i][j] = 1;
                }
            }
            vector<int> lhs, rhs;
            if (bipartiteMatching(m, lhs, rhs) != N)
                return false;
            for (int i = 0; i < N; i++) {
                out[i][lhs[i]] = a;
            }
        }

        return true;
    }

private:
    // i = row index
    // row[y] = col index in row y
    // col[x] = row index in col x
    static bool findMatch(int i, const vector<vector<int>>& mat, vector<int>& row, vector<int>& col, vector<bool>& seen) {
        for (int j = 0; j < int(mat[i].size()); j++) {
            if (mat[i][j] && !seen[j]) {
                seen[j] = true;
                if (col[j] < 0 || findMatch(col[j], mat, row, col, seen)) {
                    row[i] = j;
                    col[j] = i;
                    return true;
                }
            }
        }
        return false;
    }

    // row[y] = col index in row y
    // col[x] = row index in col x
    static int bipartiteMatching(const vector<vector<int>>& mat, vector<int>& row, vector<int>& col) {
        row = vector<int>(mat.size(), -1);
        col = vector<int>(mat[0].size(), -1);

        int ct = 0;
        for (int i = 0; i < int(mat.size()); i++) {
            vector<bool> seen(mat[0].size());
            if (findMatch(i, mat, row, col, seen))
                ct++;
        }
        return ct;
    }

    //---

    static bool dfsFindDiag(vector<int>& cnt, vector<int>& stk, int N, int K) {
        if (K < N - int(stk.size()))
            return false;
        if (K > (N - int(stk.size())) * N)
            return false;
        if (int(stk.size()) == N) {
            for (int i = 1; i <= N; i++) {
                if (cnt[i] == N - 1)
                    return false;
            }
            return true;
        }
        for (int i = 1; i <= N; i++) {
            cnt[i]++;
            stk.push_back(i);
            if (dfsFindDiag(cnt, stk, N, K - i))
                return true;
            stk.pop_back();
            cnt[i]--;
        }
        return false;
    }

    static vector<int> findDiag(int N, int K) {
        vector<int> cnt(N + 1, 0);
        vector<int> stk;

        vector<int> res;
        if (dfsFindDiag(cnt, stk, N, K)) {
            res.reserve(N);
            for (int i = 1; i <= N; i++) {
                for (int j = 0; j < cnt[i]; j++)
                    res.push_back(i);
            }
        }
        return res;
    }
};
