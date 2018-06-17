// Memoization + pruning
class Solution {
public:
    bool isScramble(string s1, int s1L, int s1R, string s2, int s2L, int s2R) {
        if (s1L == s1R)
            return s1[s1L] == s2[s2L];

        vector<int> msLL(26);
        vector<int> msLR(26);

        int cntLL = 0, cntLR = 0;
        for (int i = s1L, j = s2L, k = s2R; i < s1R && j < s2R; i++, j++, k--) {
            if (++msLL[s1[i] - 'a'] == 1)
                cntLL++;
            if (++msLR[s1[i] - 'a'] == 1)
                cntLR++;
            if (--msLL[s2[j] - 'a'] == 0)
                cntLL--;
            if (--msLR[s2[k] - 'a'] == 0)
                cntLR--;

            if (cntLL == 0) {
                if (isScramble(s1, s1L, i, s2, s2L, j) && isScramble(s1, i + 1, s1R, s2, j + 1, s2R))
                    return true;
            }
            if (cntLR == 0) {
                if (isScramble(s1, s1L, i, s2, k, s2R) && isScramble(s1, i + 1, s1R, s2, s2L, k - 1))
                    return true;
            }
        }

        return false;
    }

    bool isScramble(string s1, string s2) {
        return isScramble(s1, 0, (int)s1.length() - 1, s2, 0, (int)s2.length() - 1);
    }
};

// Memoization
class Solution {
public:
    vector<char> dp;

    bool dfs(const string& s1, int l1, int r1, const string& s2, int l2, int r2) {
        if (l1 == r1)
            return s1[l1] == s2[l2];

        int key = ((l1 * s1.length() + r1) * s1.length() + l2) * s1.length() + r2;
        if (dp[key] >= 0)
            return dp[key] > 0;

        bool res = false;
        int len = r1 - l1;
        for (int i = 0; i < len; i++) {
            if (dfs(s1, l1, l1 + i, s2, l2, l2 + i) && dfs(s1, l1 + i + 1, r1, s2, l2 + i + 1, r2) ||
                dfs(s1, l1, l1 + i, s2, r2 - i, r2) && dfs(s1, l1 + i + 1, r1, s2, l2, r2 - i - 1)) {
                res = true;
                break;
            }
        }

        return dp[key] = res;
    }

    bool isScramble(string s1, string s2) {
        if (s1.length() != s2.length())
            return false;
        dp.assign(s1.length() * s1.length() * s1.length() * s1.length(), -1);
        return dfs(s1, 0, (int)s1.length() - 1, s2, 0, (int)s2.length() - 1);
    }
};
