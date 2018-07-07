#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

#include "bitSubsets.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "bitSetSimple64.h"

static bool verify(int N, BitSubsets& bss, BitSubsets::SubsetT& subset, BitSetSimple64& bs64) {
    for (int i = 0; i < N; i++) {
        if (bss.test(subset, i) != bs64.test(i))
            return false;
    }
    return true;
}

void testBitSubsets() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Bit Subset ------------------------" << endl;
    {
        int N = 3000;
        int T = N * N;
        vector<BitSetSimple64> bs64(N);
        BitSubsets bss(N);

        auto mss = bss.createSubsets(N);

        for (int i = 0; i < N; i++)
            bs64[i].init(N);

        for (int i = 0; i < T; i++) {
            int s = RandInt32::get() % N;
            int b = RandInt32::get() % N;
            int op = RandInt32::get() % 2;
            if (op) {
                bs64[s].set(b);
                bss.set(mss[s], b);
            } else {
                bs64[s].reset(b);
                bss.reset(mss[s], b);
            }
        }

        for (int i = 0; i < N; i++) {
            if (!verify(N, bss, mss[i], bs64[i]))
                cout << "ERROR" << endl;;
            //assert(verify(N, bss, mss[i], bs64[i]));
            for (int j = 0; j < N; j++) {
                int kthBss = bss.kthBit(mss[i], j);
                int kthBs64 = bs64[i].kth(j);
                if (kthBss != kthBs64)
                    cout << "ERROR" << endl;;
                assert(kthBss == kthBs64);
            }
        }
    }
    {
        int N = 10000;
        vector<BitSetSimple64> bs64(N);
        BitSubsets bss(N);

        auto mss = bss.createSubsets(N);

        for (int i = 0; i < N; i++)
            bs64[i].init(N);

        bs64[0].set(0);
        bss.set(mss[0], 0);
        for (int i = 1; i < N; i++) {
            bs64[i] |= bs64[i - 1];
            bs64[i].set(i);

            bss.doOr(mss[i], mss[i - 1]);
            bss.set(mss[i], i);
        }

        auto subsets = bss.createSubsets(2);
        vector<BitSetSimple64> ss64(2);
        ss64[0].init(N);
        ss64[1].init(N);
        for (int i = 0; i < N; i++) {
            int x = RandInt32::get() % N;
            ss64[i & 1].set(x);
            bss.set(subsets[i & 1], x);
        }

        for (int i = 0; i < N; i++) {
            if (!verify(N, bss, mss[i], bs64[i]))
                cout << "Mismatched at " << i << endl;
            //assert(verify(N, bss, mss[i], bs64[i]));
        }

        bss.doOr(mss[7], subsets[0]);
        bss.doOr(mss[8], subsets[1]);
        bs64[7] |= ss64[0];
        bs64[8] |= ss64[1];
        if (!verify(N, bss, mss[7], bs64[7]))
            cout << "Mismatched! at Set OR #1" << endl;
        assert(verify(N, bss, mss[7], bs64[7]));
        if (!verify(N, bss, mss[8], bs64[8]))
            cout << "Mismatched! at Set OR #2" << endl;
        assert(verify(N, bss, 8, bs64[8]));

        bss.doOr(subsets[0], subsets[1]);
        bss.doOr(mss[9], subsets[0]);
        bs64[9] |= ss64[0] |= ss64[1];
        if (!verify(N, bss, mss[9], bs64[9]))
            cout << "Mismatched! at Set OR #3" << endl;
        assert(verify(N, bss, mss[9], bs64[9]));
    }
    cout << "Speed test: BitSetSimple vs BitSubset" << endl;
    {
        int N = 100000;
#ifdef _DEBUG
        N = 10000;
#endif
        {
            vector<BitSetSimple64> bs64(N);

            for (int i = 0; i < N; i++)
                bs64[i].init(N);

            PROFILE_START(0);
            bs64[0].set(0);
            for (int i = 1; i < N; i++) {
                bs64[i] |= bs64[i - 1];
                bs64[i].set(i);
            }
            PROFILE_STOP(0);

            cout << bs64.back().count() << endl;
        }
        {
            BitSubsets bss(N);
            auto mss = bss.createSubsets(N);

            PROFILE_START(1);
            bss.set(mss[0], 0);
            for (int i = 1; i < N; i++) {
                bss.doOr(mss[i], mss[i - 1]);
                bss.set(mss[i], i);
            }
            PROFILE_STOP(1);

            cout << bss.count(mss[N - 1]) << endl;
        }
    }

    cout << "OK!" << endl;
}
