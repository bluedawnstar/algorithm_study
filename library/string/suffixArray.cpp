#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//--------- Building Suffix Array ---------------------------------------------

//--- O(N (logN)^2) method ---
template <typename T>
vector<int> makeSuffixArray(T s, int n) {
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

//--- O(N logN) ---

template <typename T>
vector<int> makeSuffixArrayFast(T s, int n) {
    vector<int> SA(n);
    if (n <= 1)
        return SA;

    int m = 26;                                     //TODO: check conversion from character to rank
    vector<int> cnt(max(n, m)), currG(n), nextG(n);

    for (int i = 0; i < n; i++) {
        currG[i] = s[i] - 'a';                      //TODO: check conversion from character to rank
        ++cnt[currG[i]];
    }
    for (int i = 1; i < m; i++)
        cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--)
        SA[--cnt[currG[i]]] = i;

    int len = 1;
    do {
        int g = 0;
        for (int i = n - len; i < n; i++)
            nextG[g++] = i;
        for (int i = 0; i < n; i++) {
            if (SA[i] >= len)
                nextG[g++] = SA[i] - len;
        }

        fill(cnt.begin(), cnt.begin() + m, 0);
        for (int i = 0; i < n; i++)
            ++cnt[currG[nextG[i]]];
        for (int i = 1; i < m; i++)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--)
            SA[--cnt[currG[nextG[i]]]] = nextG[i];

        int curSA = SA[0];
        nextG[curSA] = g = 0;
        for (int i = 1; i < n; i++) {
            int prvSA = curSA;
            curSA = SA[i];

            nextG[curSA] = (prvSA + len < n && curSA + len < n
                && currG[prvSA] == currG[curSA]
                && currG[prvSA + len] == currG[curSA + len])
                ? g : ++g;
        }
        swap(currG, nextG);

        len <<= 1;
        m = g + 1;
    } while (m < n);

    return SA;
}

//--------- Calculating LCP ---------------------------------------------------

// O(N)
template <typename T>
int commonPrefix(T s, int n, int i, int j) {
    int res = 0;
    while (i < n && j < n && s[i] == s[j]) {
        i++; j++; res++;
    }
    return res;
}

// Kasai algorithm - O(N)
template <typename T>
vector<int> getAllCommonPrefix(const vector<int>& suffixArray, T s, int n) {
    vector<int> rank(n), height(n);
    for (int i = 0; i < n; i++)
        rank[suffixArray[i]] = i;

    for (int i = 0, h = 0; i < n; i++) {
        if (rank[i] > 0) {
            int j = suffixArray[rank[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h])
                h++;
            height[rank[i]] = h;
            if (h > 0)
                h--;
        }
    }

    return height;
}

//--------- Counting Distinct Substrings --------------------------------------

// O(N^2)
template <typename T>
long long countSubstrings(const vector<int>& suffixArray, T s, int n) {
    long long ans = n - suffixArray[0];
    for (int i = 1; i < n; i++) {
        int lcp = commonPrefix(s, n, suffixArray[i - 1], suffixArray[i]);
        ans += n - suffixArray[i] - lcp;
    }
    return ans;
}

// O(N)
template <typename T>
long long countSubstringsFast(const vector<int>& suffixArray, T s, int n) {
    long long ans = 0;

    vector<int> lcp = getAllCommonPrefix(suffixArray, s, n);
    for (int i = 0; i < (int)lcp.size(); i++) {
        ans += n - suffixArray[i] - lcp[i];
    }
    return ans;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testSuffixArray() {
    //return; //TODO: if you want to test string functions, make this line to a comment.

    cout << "-- makeSuffixArray() -------------" << endl;

    string S("abdadafaaabdfaeef");

    vector<int> a1 = makeSuffixArray(S, (int)S.length());
    vector<int> a2 = makeSuffixArrayFast(S, (int)S.length());

    assert(a1 == a2);

    cout << "OK!" << endl;

    cout << "-- countSubstrings() -------------" << endl;

    long long cnt11 = countSubstrings(a1, S, (int)S.length());
    long long cnt12 = countSubstrings(a2, S, (int)S.length());
    long long cnt21 = countSubstringsFast(a1, S, (int)S.length());
    long long cnt22 = countSubstringsFast(a2, S, (int)S.length());

    assert(cnt11 == cnt12);
    assert(cnt21 == cnt22);
    assert(cnt11 == cnt21);

    cout << "OK!" << endl;
}
