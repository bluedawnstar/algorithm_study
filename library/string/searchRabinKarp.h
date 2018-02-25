#pragma once

inline vector<int> searchRabinKarp(const string& text, const string& pattern) {
    const int q = 3355439;  //TODO: it can be changed with another value
    const int d = 256;      //TODO: it can be changed with another value

    int N = (int)text.length();
    int M = (int)pattern.length();

    int dM = 1;
    for (int i = 1; i <= M; i++) // pre-caculate d^M % q for subtraction
        dM = (dM * d) % q;

    int pattHash = 0, textHash = 0;
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

inline vector<int> searchRabinKarpLarge(const string& text, const string& pattern) {
    const long long q = 100000015277ll; //TODO: it can be changed with another value
    const long long d = 3355439ll;      //TODO: it can be changed with another value

    int N = (int)text.length();
    int M = (int)pattern.length();

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
