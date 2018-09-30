#pragma once

struct LexicographicallyMinimalStringRotation {
    // Booth's Algorithm, O(n)
    static int booth(string s) {
        s += s;                     // concatenate string to it self to avoid modular arithmetic

        int n = int(s.length());
        vector<int> f(n, -1);       // failure function
        int k = 0;                  // least rotation of string found so far

        for (int j = 1; j < n; j++) {
            auto cj = s[j];
            int i = f[j - k - 1];
            while (i != -1 && cj != s[k + i + 1]) {
                if (cj < s[k + i + 1])
                    k = j - i - 1;
                i = f[i];
            }
            if (cj != s[k + i + 1]) {
                if (cj < s[k])
                    k = j;
                f[j - k] = -1;
            } else
                f[j - k] = i + 1;
        }
        return k;
    }

    // O(n)
    static int booth2(const string& s) {
        int n = int(s.length());

        int i = 0, j = 1;
        while (true) {
            int k, ci, cj;
            for (k = 0; k < n; k++) {
                ci = s[(i + k) % n];
                cj = s[(j + k) % n];
                if (ci != cj)
                    break;
            }

            if (k == n)
                return min(i, j);

            if (ci > cj) {
                i += k + 1;
                i += (i == j);
            } else {
                j += k + 1;
                j += (i == j);
            }
        }

        return -1;
    }

    // Duval's Lyndon Factorization Algorithm, O(n)
    static int duval(string s) {
        s += s;
        int n = int(s.size());

        int i = 0, ans = 0;
        while (i < n / 2) {
            ans = i;
            int j = i + 1, k = i;
            while (j < n && s[k] <= s[j]) {
                if (s[k] < s[j])
                    k = i;
                else
                    k++;
                j++;
            }
            while (i <= k)
                i += j - k;
        }
        return ans;
    }
};
