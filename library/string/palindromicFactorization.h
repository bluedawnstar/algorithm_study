#pragma once

/*
  https://arxiv.org/abs/1403.2431

  A Subquadratic Algorithm for Minimum Palindromic Factorization

    pl[i].first = the number of minimal even factorization in S[0, i-1]
    pl[i].second = the number of minimal odd factorization in S[0, i-1]
    1 <= i <= n
*/
struct PalindromicFactorization {
    static const int INF = 1000000000;

    vector<pair<int, int>> pl;
    vector<pair<int, int>> gpl;

    PalindromicFactorization() {
    }

    PalindromicFactorization(const char s[], int n) {
        factorize(s, n);
    }

    PalindromicFactorization(const string& s) {
        factorize(s);
    }


    int countMinEventFactorization() const {
        return pl.back().first;
    }

    int countMinOddFactorization() const {
        return pl.back().second;
    }

    // the number of minimal even factorization in S[0, prefixLength-1]
    int countMinEventFactorization(int prefixLength) const {
        return pl[prefixLength].first;
    }

    // the number of minimal even factorization in S[0, prefixLength-1]
    int countMinOddFactorization(int prefixLength) const {
        return pl[prefixLength].second;
    }


    // O(NlogN)
    void factorize(const char s[], int n) {
        pl.resize(n + 1);
        gpl.resize(n + 1);

        for (int i = 0; i <= n; i++) {
            gpl[i].first = INF;
            gpl[i].second = INF + 1;
        }

        vector<tuple<int, int, int>> g(32);
        vector<tuple<int, int, int>> gp(32);
        vector<tuple<int, int, int>> gpp(32);

        int pg = 0;
        for (int j = 0; j < n; j++) {
            // g -> gp
            int pgp = 0;
            for (int u = 0; u < pg; ++u) {
                int i = get<0>(g[u]);
                if (i - 1 >= 0 && s[i - 1] == s[j]) {
                    --get<0>(g[u]);
                    gp[pgp++] = g[u];
                }
            }

            // gp -> gpp
            int pgpp = 0, r = -j - 2;
            for (int u = 0; u < pgp; ++u) {
                int i, d, k;
                tie(i, d, k) = gp[u];

                if (i - r != d) {
                    gpp[pgpp++] = make_tuple(i, i - r, 1);
                    if (k > 1)
                        gpp[pgpp++] = make_tuple(i + d, d, k - 1);
                } else
                    gpp[pgpp++] = make_tuple(i, d, k);
                r = i + (k - 1) * d;
            }
            if (j - 1 >= 0 && s[j - 1] == s[j]) {
                gpp[pgpp++] = make_tuple(j - 1, j - 1 - r, 1);
                r = j - 1;
            }
            gpp[pgpp++] = make_tuple(j, j - r, 1);

            // gpp -> g
            pg = 0;
            auto& front = gpp[0];
            for (int u = 1; u < pgpp; ++u) {
                auto& x = gpp[u];
                if (get<1>(x) == get<1>(front))
                    get<2>(front) += get<2>(x);
                else {
                    g[pg++] = front;
                    front = x;
                }
            }
            g[pg++] = front;

            // dp update
            if (((j + 1) & 1) == 0) {
                pl[j + 1].first = j + 1;
                pl[j + 1].second = INF + 1;
            } else {
                pl[j + 1].first = INF;
                pl[j + 1].second = j + 1;
            }

            for (int u = 0; u < pg; ++u) {
                int i, d, k;
                tie(i, d, k) = g[u];

                r = i + (k - 1) * d;
                upd(pl[j + 1], pl[r].first + 1);
                upd(pl[j + 1], pl[r].second + 1);
                if (k > 1) {
                    upd(pl[j + 1], gpl[i + 1 - d].first);
                    upd(pl[j + 1], gpl[i + 1 - d].second);
                }
                if (i + 1 >= d) {
                    if (k > 1) {
                        upd(gpl[i + 1 - d], pl[r].first + 1);
                        upd(gpl[i + 1 - d], pl[r].second + 1);
                    } else {
                        set(gpl[i + 1 - d], pl[r].first + 1);
                        set(gpl[i + 1 - d], pl[r].second + 1);
                    }
                }
            }
        }
    }

    void factorize(const string& s) {
        factorize(s.c_str(), int(s.length()));
    }

private:
    void set(pair<int, int>& pl, int val) {
        if (val <= 0)
            return;

        if (val & 1)
            pl.second = val;
        else
            pl.first = val;
    }

    void upd(pair<int, int>& pl, int val) {
        if (val <= 0)
            return;

        if (val & 1) {
            if (pl.second == -1 || pl.second > val)
                pl.second = val;
        } else {
            if (pl.first == -1 || pl.first > val)
                pl.first = val;
        }
    }
};


struct PalindromicFactorizationDP {
    static const int INF = 1000000000;

    string S;
    vector<int> C;              // C[i] = Minimum number of cuts needed for palindrome partitioning of substring str[0..i]
    vector<vector<bool>> P;     // P[i][j] = true if substring str[i..j] is palindrome, else false
                                // Note that C[i] is 0 if P[0][i] is true
    vector<int> prev;

    PalindromicFactorizationDP() {
    }

    PalindromicFactorizationDP(const char s[], int n) {
        factorize(s, n);
    }

    PalindromicFactorizationDP(const string& s) {
        factorize(s);
    }


    int countMinFactorization() const {
        return C.back() + 1;
    }

    // the number of minimal even factorization in S[0, prefixLength-1]
    int countMinFactorization(int prefixLength) const {
        return C[prefixLength - 1] + 1;
    }

    vector<string> getPalindromes() const {
        vector<string> res(C.back() + 1);
        for (int i = int(C.size()) - 1, j = int(res.size()) - 1; i >= 0; i = prev[i], j--)
            res[j].assign(S.c_str() + prev[i] + 1, i - prev[i]);
        return res;
    }

    vector<string> getPalindromes(int prefixLength) const {
        vector<string> res(C[prefixLength - 1] + 1);
        for (int i = prefixLength - 1, j = int(res.size()) - 1; i >= 0; i = prev[i], j--)
            res[j].assign(S.c_str() + prev[i] + 1, i - prev[i]);
        return res;
    }


    // O(N^2)
    void factorize(const char s[], int n) {
        S.assign(s, n);
        C = vector<int>(n);
        P = vector<vector<bool>>(n, vector<bool>(n));
        prev.resize(n);

        // every substring of length 1 is a palindrome 
        for (int i = 0; i < n; i++) {
            P[i][i] = true;
            prev[i] = i - 1;
        }

        // L is substring length.
        // Build the solution in bottom up manner by considering all substrings of length starting from 2 to n
        for (int L = 2; L <= n; L++) {
            // for substring of length L, set different possible starting indexes 
            for (int i = 0; i < n - L + 1; i++) {
                int j = i + L - 1; // ending index 

                // If L is 2, then we just need to compare two characters.
                // Else need to check two corner characters and value of P[i+1][j-1] 
                if (L == 2)
                    P[i][j] = (s[i] == s[j]);
                else
                    P[i][j] = (s[i] == s[j]) && P[i + 1][j - 1];
            }
        }

        for (int i = 0; i < n; i++) {
            if (P[0][i] == true) {
                C[i] = 0;
                prev[i] = -1;
            } else {
                C[i] = INF;
                for (int j = 0; j < i; j++) {
                    if (P[j + 1][i] == true && 1 + C[j] < C[i]) {
                        C[i] = 1 + C[j];
                        prev[i] = j;
                    }
                }
            }
        }
    }

    void factorize(const string& s) {
        factorize(s.c_str(), int(s.length()));
    }
};


// Handling All Minimum Palindromic Factorization
struct PalindromicFactorizationDPEx {
    static const int INF = 1000000000;

    string S;
    vector<int> C;              // C[i] = Minimum number of cuts needed for palindrome partitioning of substring str[0..i]
    vector<vector<bool>> P;     // P[i][j] = true if substring str[i..j] is palindrome, else false
                                // Note that C[i] is 0 if P[0][i] is true

    vector<int> count;
    vector<vector<int>> prev;

    PalindromicFactorizationDPEx() {
    }

    PalindromicFactorizationDPEx(const char s[], int n) {
        factorize(s, n);
    }

    PalindromicFactorizationDPEx(const string& s) {
        factorize(s);
    }


    int countMinFactorization() const {
        return C.back() + 1;
    }

    // the number of minimal even factorization in S[0, prefixLength-1]
    int countMinFactorization(int prefixLength) const {
        return C[prefixLength - 1] + 1;
    }


    int countWays() const {
        return count.back();
    }

    int countWays(int prefixLength) const {
        return count[prefixLength - 1];
    }


    vector<string> getPalindromes() const {
        vector<string> res(C.back() + 1);
        for (int i = int(C.size()) - 1, j = int(res.size()) - 1; i >= 0; i = prev[i][0], j--)
            res[j].assign(S.c_str() + prev[i][0] + 1, i - prev[i][0]);
        return res;
    }

    vector<string> getPalindromes(int prefixLength) const {
        vector<string> res(C[prefixLength - 1] + 1);
        for (int i = prefixLength - 1, j = int(res.size()) - 1; i >= 0; i = prev[i][0], j--)
            res[j].assign(S.c_str() + prev[i][0] + 1, i - prev[i][0]);
        return res;
    }


    vector<vector<string>> getAllPalindromes() const {
        vector<vector<string>> res;
        vector<string> tmp(C.back() + 1);
        dfs(res, int(C.size()) - 1, tmp, C.back());
        return res;
    }

    vector<vector<string>> getAllPalindromes(int prefixLength) const {
        vector<vector<string>> res;
        vector<string> tmp(C[prefixLength - 1] + 1);
        dfs(res, prefixLength - 1, tmp, C[prefixLength - 1]);
        return res;
    }

    
    // O(N^2)
    void factorize(const char s[], int n) {
        S.assign(s, n);
        C.assign(n, 0);
        P = vector<vector<bool>>(n, vector<bool>(n));
        count.assign(n, 0);
        prev = vector<vector<int>>(n);

        // every substring of length 1 is a palindrome 
        for (int i = 0; i < n; i++) {
            P[i][i] = true;
            count[i] = 1;
            prev[i].push_back(i - 1);
        }

        // L is substring length.
        // Build the solution in bottom up manner by considering all substrings of length starting from 2 to n
        for (int L = 2; L <= n; L++) {
            // for substring of length L, set different possible starting indexes 
            for (int i = 0; i < n - L + 1; i++) {
                int j = i + L - 1; // ending index 

                                   // If L is 2, then we just need to compare two characters.
                                   // Else need to check two corner characters and value of P[i+1][j-1] 
                if (L == 2)
                    P[i][j] = (s[i] == s[j]);
                else
                    P[i][j] = (s[i] == s[j]) && P[i + 1][j - 1];
            }
        }

        for (int i = 0; i < n; i++) {
            if (P[0][i] == true) {
                C[i] = 0;

                prev[i].clear();
                prev[i].push_back(-1);
                count[i] = 1;
            } else {
                C[i] = INF;
                for (int j = 0; j < i; j++) {
                    if (P[j + 1][i] == true && 1 + C[j] <= C[i]) {
                        if (1 + C[j] < C[i]) {
                            C[i] = 1 + C[j];
                            prev[i].clear();
                            count[i] = 0;
                        }
                        prev[i].push_back(j);
                        count[i] += count[j];
                    }
                }
            }
        }
    }

    void factorize(const string& s) {
        factorize(s.c_str(), int(s.length()));
    }

private:
    void dfs(vector<vector<string>>& res, int si, vector<string>& v, int vi) const {
        if (si < 0) {
            res.push_back(v);
            return;
        }
        for (auto p : prev[si]) {
            v[vi].assign(S.c_str() + p + 1, si - p);
            dfs(res, p, v, vi - 1);
        }
    }
};
