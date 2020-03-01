#pragma once

/*
  https://www.codechef.com/problems/EXPCH
*/
struct BracketChangeCounter {
    // the sum of all change count in all substrings, O(N)
    static long long countAll(const string& s) {
        int N = int(s.length());

        vector<int> oddDP(N / 2 + 1);
        vector<int> evenDP((N + 1) / 2 + 1);

        long long res = 0;

        stack<pair<int, int>> stk;
        stk.emplace(N + 1, -1);         // (depth, index of '(' or ')')

        int lastPosition = -1;          // last position of '(' or ')' in s
        int seqIndex = 0;               // sequence index of '(' or ')'
        int depth = 0;
        for (int i = 0; i < N; i++) {
            if (s[i] == '(')
                depth--;
            else if (s[i] == ')')
                depth++;

            if (s[i] == '(' || s[i] == ')') {
                int j = seqIndex >> 1;
                if (seqIndex & 1)
                    oddDP[j + 1] = oddDP[j] + (i - lastPosition);
                else
                    evenDP[j + 1] = evenDP[j] + (i - lastPosition);

                while (stk.top().first < depth)
                    stk.pop();

                if (s[i] == ')') {
                    int start = stk.top().second + 1;
                    int cnt = (seqIndex & 1) ? (oddDP[j + 1] - oddDP[start / 2])
                        : (evenDP[j + 1] - evenDP[(start + 1) / 2]);
                    res += 1ll * cnt * (N - i);
                }

                stk.emplace(depth, seqIndex++);
                lastPosition = i;
            }
        }

        return res;
    }

    // the sum of all change count in all substrings, O(N)
    static long long countAllFast(const string& s) {
        int N = int(s.length());

        vector<int> dp((N + 1) * 2);

        long long res = 0;
        for (int i = 0, depth = N; i < N; i++) {
            ++dp[depth];
            if (s[i] == '(') {
                depth--;
            } else if (s[i] == ')') {
                res += 1ll * dp[depth] * (N - i);   // cnt * (N - i)
                dp[depth + 2] += dp[depth];         // odd or even depth
                dp[depth] = 0;

                depth++;
            }
        }

        return res;
    }
};
