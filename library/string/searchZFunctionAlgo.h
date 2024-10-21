#pragma once

// O(n*C)
inline string makeSmallestAlphabetStringFromZArray(const vector<int>& z) {
    if (z.empty())
        return "";

    int n = int(z.size());

    string s(n, ' ');
    s[0] = 'a';

    int r = 0, rFlag = 0;
    for (int i = 1; i < n; i++) {
        int j = max(0, r - i);
        for (; j < z[i]; j++)
            s[i + j] = s[j];    // restore from the prefix

        if (i + j > r) {
            r = i + j;
            rFlag = 0;
        }

        if (z[i] < n)
            rFlag |= 1 << (s[z[i]] - 'a');

        // add the smallest alphabet to empty positions
        if (i >= r) {
            for (int j = 0; j < 26; j++) {
                if ((rFlag & (1 << j)) == 0) {
                    s[i] = 'a' + j;
                    break;
                }
            }
        }
    }

    return s;
}

struct ZArrayConverter {
    //--- z-array to prefix-array

    /*
        s = "ababab" 
        z  = [0, 0, 4, 0, 2 0]
        pi = [0, 0, 1, 2, 3, 4]

        pi ^
         4 |              *
         3 |           *
         2 |        *     *
         1 |     *     *
           *--*-------------->
           0  1  2  3  4  5
    */
    // O(n)
    static vector<int> buildPrefixArrayFromZArray(const vector<int>& z) {
        if (z.empty())
            return {};

        int n = int(z.size());

        vector<int> pi(n);
        pi[0] = 0;
        for (int i = 1; i < n; i++) {
            if (pi[i + z[i] - 1] <= 0)
                pi[i + z[i] - 1] = z[i];
        }

        for (int i = n - 1, j = 0; i > 0; i--, j--) {
            if (j < pi[i])
                j = pi[i];
            else
                pi[i] = j;
        }

        return pi;
    }

    //--- prefix-array to z-array

    /*
       s = "aabaab"
       pi = [ 0, 1, 0, 1, 2, 3 ]
       z  = [ 0, 1, 0, 3, 1, 0 ]

        pi ^
         4 |               
         3 |              *
         2 |           *   
         1 |  *     *   
           *-----*----------->
           0  1  2  3  4  5
    */
    // O(n^2) ?
    static vector<int> buildZArrayFromPrefixArray(const vector<int>& pi) {
        if (pi.empty())
            return {};

        int n = int(pi.size());
        vector<int> z(n);
        z[0] = 0;

        for (int i = 1; i < n; i++) {
            int j = pi[i];
            while (j > 0 && z[i - j + 1] < j) {
                z[i - j + 1] = j;
                j = pi[j - 1];
            }
        }

        return z;
    }
};
