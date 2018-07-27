#pragma once

//https://www.hackerrank.com/contests/university-codesprint-4/challenges/maximum-permutation

// lowercase alphabet only
inline vector<int> searchRabinKarp(const string& text, const string& pattern) {
    const unsigned long long p = 31ll;

    int N = int(text.length());
    int M = int(pattern.length());

    unsigned long long dM = 1;
    for (int i = 1; i <= M; i++) // pre-caculate p^M % q for subtraction
        dM = (dM * p);

    unsigned long long pattHash = 0, textHash = 0;
    for (int i = 0; i < M; i++) {
        pattHash = pattHash * p + (pattern[i] - 'a' + 1);   // hash of pattern
        textHash = textHash * p + (text[i] - 'a' + 1);      // hash of text
    }

    vector<int> res;

    for (int i = 0; i <= N - M; i++) {
        if (pattHash == textHash) {
            bool matched = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    matched = false;
                    break;
                }
            }

            if (matched)
                res.push_back(i);
        }

        // next hash value
        if (i < N - M) {
            textHash = p * textHash - (text[i] - 'a' + 1) * dM + (text[i + M] - 'a' + 1);
        }
    }

    return res;
}


inline vector<int> searchRabinKarp2(const string& text, const string& pattern) {
    const long long q = 100000015277ll; //TODO: it can be changed with another value
    const long long d = 3355439ll;      //TODO: it can be changed with another value

    int N = int(text.length());
    int M = int(pattern.length());

    long long dM = 1;
    for (int i = 1; i <= M; i++) // pre-caculate d^M % q for subtraction
        dM = (dM * d) % q;

    long long pattHash = 0, textHash = 0;
    for (int i = 0; i < M; i++) {
        pattHash = (pattHash * d + pattern[i]) % q; // hash of pattern
        textHash = (textHash * d + text[i]) % q;    // hash of text
    }

    vector<int> res;

    for (int i = 0; i <= N - M; i++) {
        if (pattHash == textHash) {
            bool matched = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    matched = false;
                    break;
                }
            }

            if (matched)
                res.push_back(i);
        }

        // next hash value
        if (i < N - M) {
            textHash = (d * textHash - text[i] * dM + text[i + M]) % q;
            if (textHash < 0)
                textHash += q;
        }
    }

    return res;
}
