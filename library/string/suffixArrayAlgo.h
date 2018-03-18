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

//--- O(N (logN)^2) method ---
template <typename T>
vector<int> makeSuffixArrayNaive(T s, int n) {
    // A structure to store suffixes and their indexes
    vector<pair<int, int>> suffixes(n);     // (current rank, next rank)
    vector<int> SA(n);

    auto cmp = [&suffixes](int a, int b) {
        return (suffixes[a].first != suffixes[b].first)
            ? suffixes[a].first < suffixes[b].first
            : suffixes[a].second < suffixes[b].second;
    };

    for (int i = 0; i < n; i++)
        SA[i] = i;

    // Store suffixes and their indexes in an array of structures.
    // The structure is needed to sort the suffixes alphabatically
    // and maintain their old indexes while sorting
    for (int i = 0; i < n; i++) {
        suffixes[i].first = s[i] - 'a';                             //TODO: check conversion from character to rank
        suffixes[i].second = ((i + 1) < n) ? (s[i + 1] - 'a') : -1; //TODO: check conversion from character to rank
    }

    // Sort the suffixes using the comparison function defined above.
    sort(SA.begin(), SA.end(), cmp);

    for (int k = 4; k < 2 * n; k = k * 2) {
        // Assigning rank and index values to first suffix
        int rank = 0;
        int prevRank = suffixes[SA[0]].first;
        suffixes[SA[0]].first = rank;

        // Assigning rank to suffixes
        for (int i = 1; i < n; i++) {
            int curI = SA[i];
            int prvI = SA[i - 1];
            // If first rank and next ranks are same as that of previous
            // suffix in array, assign the same new rank to this suffix
            if (suffixes[curI].first == prevRank &&
                suffixes[curI].second == suffixes[prvI].second) {
                prevRank = suffixes[curI].first;
                suffixes[curI].first = rank;
            } else { // Otherwise increment rank and assign
                prevRank = suffixes[curI].first;
                suffixes[curI].first = ++rank;
            }
        }

        // Assign next rank to every suffix
        for (int i = 0; i < n; i++) {
            int nextI = SA[i] + k / 2;
            suffixes[SA[i]].second = (nextI < n) ? suffixes[nextI].first : -1;
        }

        // Sort the suffixes according to first k characters
        sort(SA.begin(), SA.end(), cmp);
    }

    // Return the suffix array
    return SA;
}
