#pragma once

struct SimpleSubstring {
    const char* s;      // string pointer
    int         len;    // length
    size_t      h;      // hash value

    SimpleSubstring(const char* s, int len, size_t h) : s(s), len(len), h(h) {
    }

    bool operator <(const SimpleSubstring& rhs) const {
        int cmp = memcmp(s, rhs.s, min(len, rhs.len));
        if (cmp)
            return cmp < 0;
        return len < rhs.len;
    }

    bool operator ==(const SimpleSubstring& rhs) const {
        int cmp = memcmp(s, rhs.s, min(len, rhs.len));
        if (cmp)
            return false;
        return len == rhs.len;
    }
};

namespace std
{
    template<> struct hash<SimpleSubstring>
    {
        typedef SimpleSubstring argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept {
            return s.h;
        }
    };
}

struct SimpleSubstringBuilder {
    static const unsigned long long P1 = 31ll;
    static const unsigned long long P2 = 239017ll;

    int maxLen;                     // max length
    vector<unsigned long long> D1;
    vector<unsigned long long> D2;

    void init(int maxN) {
        maxLen = maxN;
        D1.resize(maxN + 1);
        D2.resize(maxN + 1);

        D1[0] = 1;
        D2[0] = 1;
        for (int i = 1; i <= maxN; i++) {
            D1[i] = D1[i - 1] * P1;
            D2[i] = D2[i - 1] * P2;
        }
    }

    // per each string

    vector<unsigned long long> H1;
    vector<unsigned long long> H2;

    void prepare(const char* s, int n) {
        H1.resize(n + 1);
        H2.resize(n + 1);

        H1[0] = 0;
        H2[0] = 0;
        for (int i = 0; i < n; i++) {
            H1[i + 1] = H1[i] * P1 + s[i];
            H2[i + 1] = H2[i] * P2 + s[i];
        }
    }

    void prepare(const string& s) {
        return prepare(s.c_str(), int(s.length()));
    }


    unsigned long long substrHash(int i, int len) const {
        unsigned long long h1 = H1[i + len] - H1[i] * D1[len];
        unsigned long long h2 = H2[i + len] - H2[i] * D2[len];
        return h1 * h2;
    }
};
