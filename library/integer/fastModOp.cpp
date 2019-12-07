#include <vector>
#include <algorithm>

using namespace std;

#include "fastModOp.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <time.h>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define MOD 1000000007

static int permSlow(int n, int r) {
    long long res = 1;

    while (r-- > 0)
        res = (res * n--) % MOD;
    
    return int(res);
}

static int combSlow(int n, int r) {
    return int(1ll * permSlow(n, r) * FastModOp<MOD>::modInv(permSlow(r, r)) % MOD);
}

void testFastModOp() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast Modular Operation -------------------------" << endl;
    {
        int N = 30000;
#ifdef _DEBUG
        N = 10000;
#endif
        FastModOp<MOD> modOp(N);
        for (int r = 0; r <= N; r++) {
            //--- factorial
            {
                int gt = permSlow(N, N);
                int ans = modOp.fact(N);
                if (gt != ans)
                    cout << "Mismatched at line " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(gt == ans);

                gt = FastModOp<MOD>::modInv(gt);
                ans = modOp.factInv(N);
                if (gt != ans)
                    cout << "Mismatched at line " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(gt == ans);
            }
            //--- permutation
            {
                int gt = permSlow(N, r);
                int ans = modOp.perm(N, r);
                if (gt != ans)
                    cout << "Mismatched at line " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(gt == ans);

                gt = FastModOp<MOD>::modInv(gt);
                ans = modOp.permInv(N, r);
                if (gt != ans)
                    cout << "Mismatched at line " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(gt == ans);
            }
            //--- combination
            {
                int gt = combSlow(N, r);
                int ans = modOp.comb(N, r);
                if (gt != ans)
                    cout << "Mismatched at line " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(gt == ans);

                gt = FastModOp<MOD>::modInv(gt);
                ans = modOp.combInv(N, r);
                if (gt != ans)
                    cout << "Mismatched at line " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(gt == ans);
            }
        }
    }

    cout << "OK!" << endl;
}
