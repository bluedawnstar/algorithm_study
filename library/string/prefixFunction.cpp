#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<int> prefixFunction(string s) {
    vector<int> p(s.size());
    int j = 0;
    for (int i = 1; i < (int)s.size(); i++) {
        while (j > 0 && s[j] != s[i])
            j = p[j - 1];

        if (s[j] == s[i])
            j++;
        p[i] = j;
    }
    return p;
}

vector<int> prefixFunction(string s, int start) {
    int n = (int)s.size() - start;

    vector<int> p(n);
    int j = 0;
    for (int i = 1; i < n; i++) {
        while (j > 0 && s[j + start] != s[i + start])
            j = p[j - 1];

        if (s[j + start] == s[i + start])
            j++;
        p[i] = j;
    }
    return p;
}


// (pattern length, pattern count)
pair<int,int> checkRepeatedString(const vector<int>& prefix, int len) {
    if (prefix.empty())
        return make_pair(-1, -1);

    int n = len - prefix[len - 1];
    if (prefix[len - 1] && prefix[len - 1] % n == 0)
        return make_pair(n, len / n);

    return make_pair(-1, -1);
}

pair<int, int> checkRepeatedString(string s) {
    return checkRepeatedString(prefixFunction(s), (int)s.length());
}


// find maximum repeated prefix string (ex: "abababY" => (1, 2, 3))
// (pattern length, pattern count)
pair<int, int> getMaxRepeatedPrefixString(const vector<int>& prefix) {
    if (!prefix.empty()) {
        for (int i = (int)prefix.size() - 1; i >= 0; i--) {
            int n = i + 1 - prefix[i];
            if (prefix[i] && prefix[i] % n == 0)
                return make_pair(n, (i + 1) / n);
        }
    }

    return make_pair(-1, -1);
}

pair<int, int> getMaxRepeatedPrefixString(string s, int start) {
    return getMaxRepeatedPrefixString(prefixFunction(s, start));
}


// find maximum repeated string (ex: "XabababY" => (1, 2, 3))
// (first pos, (pattern length, pattern count))
pair<int, pair<int, int>> getMaxRepeatedString(string s) {
    int repLen = 0;
    pair<int, pair<int, int>> res(-1, pair<int, int>(-1, -1));
    for (int i = 0; i < (int)s.length(); i++) {
        int len = (int)s.length() - i;
        if (len <= repLen)
            break;

        vector<int> pref = prefixFunction(s, i);
        pair<int, int> p = getMaxRepeatedPrefixString(pref);
        if (p.first > 0 && p.first * p.second > repLen) {
            repLen = p.first * p.second;
            res.first = i;
            res.second = p;
        }
    }

    return res;
}


///////////////////////////////////////////////////////////////////////////////

#include <iostream>

ostream& operator <<(ostream& os, const pair<int,int>& rhs) {
    cout << "(" << rhs.first << ", " << rhs.second << ")";
    return os;
}

ostream& operator <<(ostream& os, const pair<int,pair<int, int>>& rhs) {
    cout << "(" << rhs.first << ", " << rhs.second.first << ", " << rhs.second.second << ")";
    return os;
}

void testPrefixFunction() {
    return; // skip this test...

    cout << "-- getRepeatedPattern() ---------" << endl;            
    cout << checkRepeatedString("abcdefg") << endl;                 // (-1, -1)
    cout << checkRepeatedString("abcabc") << endl;                  // (3, 2)
    cout << checkRepeatedString("abacdacdb") << endl;               // (-1, -1)
    cout << checkRepeatedString("aaaaaaaa") << endl;                // (1, 8)
    cout << checkRepeatedString("abcabcabcabc") << endl;            // (3, 4)
    cout << "-- findMaxRepeatedString() ---------" << endl;
    cout << getMaxRepeatedString("abcdefg") << endl;                // (-1, -1, -1)
    cout << getMaxRepeatedString("ababcabcab") << endl;             // (2, (3, 2))
    cout << getMaxRepeatedString("abacdacdb") << endl;              // (2, (3, 2))
    cout << getMaxRepeatedString("aaaaaaaa") << endl;               // (0, (1, 8))
    cout << getMaxRepeatedString("xabcabcabcabcx") << endl;         // (1, (3, 4))
}
