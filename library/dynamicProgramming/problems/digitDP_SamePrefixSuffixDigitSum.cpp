#include <vector>
#include <algorithm>

using namespace std;

#include "digitDP_SamePrefixSuffixDigitSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"
#include "../../common/rand.h"

static bool isOK(int x) {
    vector<int> digits;
    while (x) {
        digits.push_back(x % 10);
        x /= 10;
    }
    int n = int(digits.size());
    if (n & 1)
        return false;

    int prefSum = 0, suffSum = 0;
    for (int i = 0; i < n / 2; i++)
        suffSum += digits[i];
    for (int i = n / 2; i < n; i++)
        prefSum += digits[i];

    return suffSum == prefSum;
}

static int count(int L, int R) {
    int res = 0;

    for (; L <= R; L++)
        res += isOK(L);

    return res;
}

void testDigitDP_SamePrefixSuffixDigitSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- DigitDP - SamePrefixSuffixDigitSum ----------------------" << endl;
    {
        const int T = 100;
#ifdef _DEBUG
        const int MAXX = 10'000;
#else
        const int MAXX = 1'000'000;
#endif

        DigitDP_SamePrefixSuffixDigitSum counter;

        for (int tt = 0; tt < T; tt++) {
            int L = RandInt32::get() % MAXX;
            int R = RandInt32::get() % MAXX;
            if (L > R)
                swap(L, R);

            auto ans = counter.count(L, R);
            auto gt = count(L, R);

            if (ans != gt) {
                cout << "Mismatched : (" << L << ", " << R << ")" << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
