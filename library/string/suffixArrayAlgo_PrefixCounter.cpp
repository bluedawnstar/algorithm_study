#include <cmath>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;

#include "suffixArrayAlgo_PrefixCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static string makeString(int length, int charCount) {
    string res;
    for (int i = 0; i < length; i++)
        res.push_back('a' + RandInt32::get() % charCount);
    return res;
}

static string makeRepeatString(int length, int charCount) {
    string s;
    for (int i = 0; i < charCount; i++)
        s.push_back('a' + i);

    string res;
    for (int i = 0, n = length / charCount; i < n; i++)
        res += s;

    int r = length % charCount;
    if (r)
        res += s.substr(0, r);

    return res;
}


static int countPrefix(const string& s, int prefixLength, int L, int R) {
    if (R - L + 1 < prefixLength)
        return 0;

    int res = 0;
    auto t = s.substr(0, prefixLength);
    for (int i = L; i + prefixLength - 1 <= R; i++) {
        res += (t == s.substr(i, prefixLength));
    }
    return res;
}

void testSuffixArrayAlgoPrefixCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Suffix Array algorithm - Prefix Counter -----------------" << endl;
    {
        for (int CHAR_N = 1; CHAR_N <= 3; CHAR_N++) {
            const int N = CHAR_N * 5;
            auto s = makeRepeatString(N, CHAR_N);

            PrefixCounter pc;
            pc.build(s);

            for (int len = 1; len < 5; len++) {
                for (int L = 0; L < N; L++) {
                    for (int R = L; R < N; R++) {
                        int gt = countPrefix(s, len, L, R);
                        int ans = pc.query(len, L, R);
                        if (gt != ans) {
                            cout << "Mismatched : (" << L << ", " << R << ") = " << ans << ", " << gt << endl;
                            cout << " s = " << s << endl;
                        }
                        assert(gt == ans);
                    }
                }
            }
        }
    }
    {
        auto rep1 = makeRepeatString(5, 1);
        auto rep2 = makeRepeatString(5, 2);

        auto s = rep1 + rep2 + rep1 + rep2;
        const int N = int(s.length());

        PrefixCounter pc;
        pc.build(s);

        for (int len = 1; len < 5; len++) {
            for (int L = 0; L < N; L++) {
                for (int R = L; R < N; R++) {
                    int gt = countPrefix(s, len, L, R);
                    int ans = pc.query(len, L, R);
                    if (gt != ans) {
                        cout << "Mismatched : (" << L << ", " << R << ") = " << ans << ", " << gt << endl;
                        cout << " s = " << s << endl;
                    }
                    assert(gt == ans);
                }
            }
        }
    }
    {
        auto rep1 = makeRepeatString(5, 2);
        auto rep2 = makeRepeatString(5, 1);

        auto s = rep1 + rep2 + rep1 + rep2;
        const int N = int(s.length());

        PrefixCounter pc;
        pc.build(s);

        for (int len = 1; len < 5; len++) {
            for (int L = 0; L < N; L++) {
                for (int R = L; R < N; R++) {
                    int gt = countPrefix(s, len, L, R);
                    int ans = pc.query(len, L, R);
                    if (gt != ans) {
                        cout << "Mismatched : (" << L << ", " << R << ") = " << ans << ", " << gt << endl;
                        cout << " s = " << s << endl;
                    }
                    assert(gt == ans);
                }
            }
        }
    }
    {
        for (int CHAR_N = 2; CHAR_N <= 3; CHAR_N++) {
            auto rep1 = makeRepeatString(CHAR_N * 2, CHAR_N);
            auto rep2 = makeRepeatString(CHAR_N * 2, CHAR_N - 1);

            auto s = rep1 + rep2 + rep1 + rep2;
            const int N = int(s.length());

            PrefixCounter pc;
            pc.build(s);

            for (int len = 1; len < 5; len++) {
                for (int L = 0; L < N; L++) {
                    for (int R = L; R < N; R++) {
                        int gt = countPrefix(s, len, L, R);
                        int ans = pc.query(len, L, R);
                        if (gt != ans) {
                            cout << "Mismatched : (" << L << ", " << R << ") = " << ans << ", " << gt << endl;
                            cout << " s = " << s << endl;
                        }
                        assert(gt == ans);
                    }
                }
            }
        }
    }
    {
        for (int CHAR_N = 1; CHAR_N <= 3; CHAR_N++) {
            int N = 100;
#ifdef _DEBUG
            N = 50;
#endif
            auto s = makeString(N, CHAR_N);

            PrefixCounter pc;
            pc.build(s);

            for (int len = 1; len < 5; len++) {
                for (int L = 0; L < N; L++) {
                    for (int R = L; R < N; R++) {
                        int gt = countPrefix(s, len, L, R);
                        int ans = pc.query(len, L, R);
                        if (gt != ans) {
                            cout << "Mismatched : (" << L << ", " << R << ") = " << ans << ", " << gt << endl;
                            cout << " s = " << s << endl;
                        }
                        assert(gt == ans);
                    }
                }
            }
        }
    }

    cout << "OK!" << endl;
}
