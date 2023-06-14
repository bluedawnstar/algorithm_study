#pragma once

#include "prefixAutomaton.h"

struct PrefixAutomatonAlgo {
    //--- memoization

    // modify the '?' in the string T to maximize the number of occurrences of the string S, O(|S||T|)
    // - return the number of occurrences
    static int maximizeOccurrences(const string& s, const string& t) {
        PrefixAutomaton<> prefAut(s);

        int n = int(t.length());
        int m = int(s.length());
        vector<vector<int>> dp(n, vector<int>(m + 1, -1));

        function<int(int, int)> rec;
        rec = [&](int i, int j) -> int {
            if (i == n)
                return (j == m);

            if (dp[i][j] != -1)
                return dp[i][j];

            dp[i][j] = 0;
            if (t[i] == '?') {
                for (int c = 0; c < 26; c++)
                    dp[i][j] = max(dp[i][j], rec(i + 1, prefAut.automaton[j][c]));
            } else {
                dp[i][j] = rec(i + 1, prefAut.automaton[j][prefAut.ch2i(t[i])]);
            }
            dp[i][j] += (j == m);

            return dp[i][j];
        };

        return rec(0, 0);
    }

    // modify the '?' in the string T to maximize the number of occurrences of the string S, O(|S||T|)
    // - return { modified T, the number of occurrences }
    static pair<string, int> maximizeOccurrences2(const string& s, const string& t) {
        PrefixAutomaton<> prefAut(s);

        int n = int(t.length());
        int m = int(s.length());
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, -1));
        vector<vector<int>> dpCh(n, vector<int>(m + 1));        // [i][j] = selected character

        function<int(int, int)> rec;
        rec = [&](int i, int j) -> int {
            if (i == n)
                return (j == m);

            if (dp[i][j] != -1)
                return dp[i][j];

            dp[i][j] = 0;
            if (t[i] == '?') {
                int maxVal = -1;
                int maxCh = 0;
                for (int c = 0; c < 26; c++) {
                    int x = rec(i + 1, prefAut.automaton[j][c]);
                    if (maxVal < x) {
                        maxVal = x;
                        maxCh = c;
                    }
                }
                dp[i][j] = maxVal;
                dpCh[i][j] = maxCh;
            } else {
                dp[i][j] = rec(i + 1, prefAut.automaton[j][prefAut.ch2i(t[i])]);
                dpCh[i][j] = prefAut.ch2i(t[i]);
            }
            dp[i][j] += (j == m);

            return dp[i][j];
        };
        rec(0, 0);

        string t2 = t;
        for (int i = 0, j = 0; i < n; i++) {
            t2[i] = prefAut.i2ch(dpCh[i][j]);
            j = prefAut.automaton[j][dpCh[i][j]];
        }

        return make_pair(t2, dp[0][0]);
    }

    //--- DP

    // modify the '?' in the string T to maximize the number of occurrences of the string S, O(|S||T|)
    // - return the number of occurrences
    static int maximizeOccurrencesDP(const string& s, const string& t) {
        PrefixAutomaton<> prefAut(s);

        int n = int(t.length());
        int m = int(s.length());
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        dp[n][m] = 1;
        for (int i = n - 1; i >= 0; i--) {
            if (t[i] == '?') {
                for (int j = 0; j <= m; j++) {
                    int maxVal = 0;
                    for (int c = 0; c < 26; c++)
                        maxVal = max(maxVal, dp[i + 1][prefAut.automaton[j][c]]);
                    dp[i][j] = maxVal + (j == m);
                }
            } else {
                for (int j = 0; j <= m; j++)
                    dp[i][j] = dp[i + 1][prefAut.automaton[j][prefAut.ch2i(t[i])]] + (j == m);
            }
        }

        return dp[0][0];
    }

    // modify the '?' in the string T to maximize the number of occurrences of the string S, O(|S||T|)
    // - return { modified T, the number of occurrences }
    static pair<string, int> maximizeOccurrencesDP2(const string& s, const string& t) {
        PrefixAutomaton<> prefAut(s);

        int n = int(t.length());
        int m = int(s.length());
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        vector<vector<int>> dpCh(n, vector<int>(m + 1));        // [i][j] = selected character

        dp[n][m] = 1;
        for (int i = n - 1; i >= 0; i--) {
            if (t[i] == '?') {
                for (int j = 0; j <= m; j++) {
                    int maxVal = -1;
                    int maxCh = 0;
                    for (int c = 0; c < 26; c++) {
                        int x = dp[i + 1][prefAut.automaton[j][c]];
                        if (maxVal < x) {
                            maxVal = x;
                            maxCh = c;
                        }
                    }
                    dp[i][j] = maxVal + (j == m);
                    dpCh[i][j] = maxCh;
                }
            } else {
                for (int j = 0; j <= m; j++) {
                    dp[i][j] = dp[i + 1][prefAut.automaton[j][prefAut.ch2i(t[i])]] + (j == m);
                    dpCh[i][j] = prefAut.ch2i(t[i]);
                }
            }
        }

        string t2 = t;
        for (int i = 0, j = 0; i < n; i++) {
            t2[i] = prefAut.i2ch(dpCh[i][j]);
            j = prefAut.automaton[j][dpCh[i][j]];
        }

        return make_pair(t2, dp[0][0]);
    }
};
