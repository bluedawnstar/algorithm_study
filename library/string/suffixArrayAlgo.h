#pragma once

struct SuffixArrayAlgo {
    // O(N)
    static int commonPrefixNaive(const char* s, int n, int i, int j) {
        int res = 0;
        while (i < n && j < n && s[i] == s[j]) {
            i++; j++; res++;
        }
        return res;
    }

    static int commonPrefixNaive(const string& s, int i, int j) {
        return commonPrefixNaive(&s[0], (int)s.length(), i, j);
    }

    // O(N^2)
    static long long countSubstringsNaive(const vector<int>& suffixArray, const char* s, int n) {
        long long ans = n - suffixArray[0];
        for (int i = 1; i < n; i++) {
            int lcp = commonPrefixNaive(s, n, suffixArray[i - 1], suffixArray[i]);
            ans += n - suffixArray[i] - lcp;
        }
        return ans;
    }

    static long long countSubstringsNaive(const vector<int>& suffixArray, const string& s) {
        return countSubstringsNaive(suffixArray, &s[0], (int)s.length());
    }

    // O(N)
    static long long countSubstrings(const vector<int>& suffixArray, const char* s, int n) {
        long long ans = 0;

        vector<int> lcp = SuffixArray::buildLcpArray(suffixArray, s, n);
        for (int i = 0; i < (int)lcp.size(); i++) {
            ans += n - suffixArray[i] - lcp[i];
        }
        return ans;
    }

    static long long countSubstrings(const vector<int>& suffixArray, const string& s) {
        return countSubstrings(suffixArray, &s[0], (int)s.length());
    }
};
