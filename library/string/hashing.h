#pragma once

struct StringHash {
    static const int P = 31;

    static unsigned long long hash(const char* s, int len) {
        unsigned long long res = 0, pow = 1;
        for (int i = 0; i < len; i++) {
            res += (s[i] - 'a' + 1) * pow;
            pow *= P;
        }
        return res;
    }

    static unsigned long long hash(const string& s) {
        return hash((const char*)&s[0], int(s.length()));
    }
};


struct StringHashAlgo {
    static vector<vector<int>> groupIdenticalStrings(const vector<string>& s) {
        int n = int(s.size());
        vector<pair<long long, int>> hashes(n);
        for (int i = 0; i < n; i++)
            hashes[i] = { StringHash::hash(s[i]), i };

        sort(hashes.begin(), hashes.end());

        vector<vector<int>> groups;
        for (int i = 0; i < n; i++) {
            if (i == 0 || hashes[i].first != hashes[i - 1].first)
                groups.emplace_back();
            groups.back().push_back(hashes[i].second);
        }
        return groups;
    }


    static int countUniqueSubstrings(const char* s, int n) {
        const int P = 31;

        vector<unsigned long long> pow(n);
        pow[0] = 1;
        for (int i = 1; i < n; i++)
            pow[i] = pow[i - 1] * P;

        vector<unsigned long long> h(n + 1, 0);
        for (int i = 0; i < n; i++)
            h[i + 1] = h[i] + (s[i] - 'a' + 1) * pow[i];

        int cnt = 0;
        for (int l = 1; l <= n; l++) {
            unordered_set<unsigned long long> hs;
            for (int i = 0; i <= n - l; i++)
                hs.insert((h[i + l] - h[i]) * pow[n - i - 1]);
            cnt += int(hs.size());
        }

        return cnt;
    }

    static int countUniqueSubstrings(const string& s) {
        return countUniqueSubstrings(&s[0], int(s.length()));
    }
};
