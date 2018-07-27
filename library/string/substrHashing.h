#pragma once

struct SubstrHash {
    static const unsigned long long P = 239017ll;

    vector<unsigned long long> h;

    SubstrHash() {
    }

    SubstrHash(const char* s, int n) {
        build(s, n);
    }

    explicit SubstrHash(const string &s) {
        build(s);
    }


    void build(const char* s, int n) {
        h = vector<unsigned long long>(n + 1);
        h[0] = 0;
        for (int i = 0; i < n; i++)
            h[i + 1] = h[i] * P + s[i];

        D().reserve(n);
        while (int(D().size()) <= n)
            D().push_back(D().back() * P);
    }

    void build(const string& s) {
        build(&s[0], int(s.length()));
    }


    unsigned long long substr(int i, int len) const {
        return h[i + len] - h[i] * D()[len];
    }


private:
    static vector<unsigned long long>& D() {
        static vector<unsigned long long> deg(1, 1);
        return deg;
    }
};

struct CountStringsToHavePattern {
    // queries: to find the number of strings in range [left string, right string] - (start string id, end string id, pattern)
    static vector<int> solve(const vector<string>& strs, const vector<tuple<int,int,string>>& queries) {
        //step1: hashing
        vector<SubstrHash> hasher(strs.size());
        for (int i = 0; i < int(strs.size()); i++)
            hasher[i].build(strs[i]);

        //step2: group queries by pattern length
        int maxLen = 0;
        for (auto& q : queries)
            maxLen = max(maxLen, int(get<2>(q).length()));

        vector<vector<int>> queryGroup(maxLen + 1);
        for (int i = 0; i < int(queries.size()); i++)
            queryGroup[get<2>(queries[i]).length()].push_back(i);

        // step3: process query groups
        vector<int> res(queries.size());
        for (int L = 1; L <= maxLen; L++) {
            if (queryGroup[L].empty())
                continue;

            unordered_map<unsigned long long, vector<int>> M;
            for (int i = 0; i < int(strs.size()); i++) {
                for (int j = 0; j + L <= int(strs[i].length()); j++) {
                    auto& v = M[hasher[i].substr(j, L)];
                    if (v.empty() || v.back() != i) // check if strs[i] has duplicate patterns
                        v.push_back(i);
                }
            }

            for (int i : queryGroup[L]) {
                auto L = get<0>(queries[i]);
                auto R = get<1>(queries[i]);
                auto& s = get<2>(queries[i]);
                auto& v = M[SubstrHash(s).substr(0, int(s.length()))];
                res[i] = int(upper_bound(v.begin(), v.end(), R) - lower_bound(v.begin(), v.end(), L));
            }
        }

        return res;
    }
};
