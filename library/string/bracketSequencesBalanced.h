#pragma once

// https://cp-algorithms.com/combinatorics/bracket_sequences.html

inline bool isBalancedSequence(string& s) {
    int n = int(s.length());
    if (n & 1)
        return false;

    int depth = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '(')
            depth++;
        else {
            depth--;
            if (depth < 0)
                return false;
        }
    }

    return depth == 0;
}

inline bool nextBalancedSequence(string& s) {
    int n = int(s.size());
    int depth = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (s[i] == '(')
            depth--;
        else
            depth++;

        if (s[i] == '(' && depth > 0) {
            depth--;
            int open = (n - i - 1 - depth) / 2;
            int close = n - i - 1 - open;
            string next = s.substr(0, i) + ')' + string(open, '(') + string(close, ')');
            s.swap(next);
            return true;
        }
    }

    return false;
}

template <typename T>
struct BalancedBracketSequences {
    vector<vector<T>> dp;

    BalancedBracketSequences() {
    }

    explicit BalancedBracketSequences(int n) {
        prepare(n);
    }

    void prepare(int n) {
#if 0
        int half = n >> 1;

        dp = vector<vector<T>>(n + 1, vector<T>(half + 1, 0));
        dp[0][0] = 1;
        for (int i = 1; i <= n; i++) {
            dp[i][0] = dp[i - 1][1];
            for (int j = 1; j < half; j++)
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j + 1];
            dp[i][half] = dp[i - 1][half - 1];
        }
#else
        dp = vector<vector<T>>(n + 1, vector<T>(n + 1, 0));
        dp[0][0] = 1;
        for (int i = 1; i <= n; i++) {
            dp[i][0] = dp[i - 1][1];
            for (int j = 1; j < n; j++)
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j + 1];
            dp[i][n] = dp[i - 1][n - 1];
        }
#endif
    }

    // '(', ')'
    // 1 <= kth
    string kthBalanced(int n, int kth) {
        string ans;
        if (n & 1)
            return ans;

        int half = n >> 1;

        int depth = 0;
        for (int i = 0; i < n; i++) {
            if (depth + 1 <= half && dp[n - i - 1][depth + 1] >= kth) {
                ans += '(';
                depth++;
            } else {
                ans += ')';
                if (depth + 1 <= half)
                    kth -= dp[n - i - 1][depth + 1];
                depth--;
            }
        }
        return ans;
    }

    // '(', ')' and '[', ']'
    // 1 <= kth
    string kthBalanced2(int n, int kth) {
        string ans;
        if (n & 1)
            return ans;

        int half = n >> 1;

        int depth = 0;
        stack<char> st;
        for (int i = 0; i < n; i++) {
            // '('
            if (depth + 1 <= half) {
                int cnt = dp[n - i - 1][depth + 1] << ((n - i - 1 - depth - 1) / 2);
                if (cnt >= kth) {
                    ans += '(';
                    st.push('(');
                    depth++;
                    continue;
                }
                kth -= cnt;
            }

            // ')'
            if (depth && st.top() == '(') {
                int cnt = dp[n - i - 1][depth - 1] << ((n - i - 1 - depth + 1) / 2);
                if (cnt >= kth) {
                    ans += ')';
                    st.pop();
                    depth--;
                    continue;
                }
                kth -= cnt;
            }

            // '['
            if (depth + 1 <= half) {
                int cnt = dp[n - i - 1][depth + 1] << ((n - i - 1 - depth - 1) / 2);
                if (cnt >= kth) {
                    ans += '[';
                    st.push('[');
                    depth++;
                    continue;
                }
                kth -= cnt;
            }

            // ']'
            ans += ']';
            st.pop();
            depth--;
        }
        return ans;
    }

    //--- almost balanced sequences

    // https://www.hackerearth.com/problem/algorithm/almost-correct-bracket-sequence-30c3bcde/
    // "a string that consists of only brackets that can be turned into a correct bracket sequence
    //  by removing exactly one bracket from it"
    // 1 <= kth
    string kthAlmostBalanced(int n, T kth) {
        string ans;

        if ((n & 1) == 0)
            return ans;

        T total = countAlmostBalanced(n);
        if (total < kth)
            return ans;

        --kth;

        ans.assign(n, '(');

        vector<int> b(n + 2);
        vector<vector<bool>> good(n + 2, vector<bool>(n + 2));

        for (int i = 1; i <= n + 1; i++)
            good[i][i - 1] = true;

        for (int i = 1; i <= n; i++) {
            b[i] = b[i - 1] + 1;
            for (int j = 1; j <= i; ++j)
                good[j][i] = (good[j][i - 1] && (b[i] - b[j - 1]) >= 0);

            T cur = 0;
            for (int j = 1; j <= i; ++j)
                if (good[1][j - 1] && b[j - 1] == 0 && good[j + 1][i])
                    cur += dp[n - i][b[i] - b[j]];

            if (good[1][i]) {
                for (int j = i + 1; j <= n; ++j)
                    cur += 2 * dp[j - i - 1][b[i]] * dp[n - j][0];
            }

            if (cur <= kth) {
                kth -= cur;
                ans[i - 1] = ')';
                b[i] = b[i - 1] - 1;
                for (int j = 1; j <= i; j++)
                    good[j][i] = (good[j][i - 1] && (b[i] - b[j - 1]) >= 0);
            }
        }

        return ans;
    }

//private:
    T countAlmostBalanced(int n) {
        T res = 0;
        for (int i = 0; i <= n; i += 2)
            res += 2 * dp[i][0] * dp[n - 1 - i][0];
        return res;
    }
};
