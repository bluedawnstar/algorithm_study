#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "offlineRMQ.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "segmentTreeRMQ.h"

static int getMin(const vector<int>& v, int L, int R) {
    int res = v[L];
    while (++L <= R) {
        if (res > v[L])
            res = v[L];
    }
    return res;
}

void testOfflineRMQ() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Off-line RMQ ------------------------" << endl;
    {
        int N = 10000;
        int T = 10000;
        
        vector<int> value(N);
        for (int i = 0; i < N; i++)
            value[i] = RandInt32::get();
        
        vector<pair<int, int>> query(T);
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            query[i].first = L;
            query[i].second = R;
        }

        OfflineRMQ<int> rmq;
        auto ans = rmq.solve(value, query);
        for (int i = 0; i < T; i++) {
            int gt = getMin(value, query[i].first, query[i].second);
            if (ans[i] != gt)
                cout << "Mismatch at " << i << ": " << ans[i] << ", " << gt << endl;
            assert(ans[i] == gt);
        }
    }
    cout << "*** Speed test ***" << endl;
    {
        int N = 10000;
        int T = 10000;
#ifndef _DEBUG
        N = 1000000;
        T = 1000000;
#endif

        vector<int> value(N);
        for (int i = 0; i < N; i++)
            value[i] = RandInt32::get();

        vector<pair<int, int>> query(T);
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            query[i].first = L;
            query[i].second = R;
        }

        PROFILE_START(0);
        {
            RMQ rmq(value);

            int ansSum = 0;
            for (auto q : query)
                ansSum += rmq.query(q.first, q.second);

            cout << ansSum << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            OfflineRMQ<int> rmq;
            auto ans = rmq.solve(value, query);

            int ansSum = 0;
            for (auto x : ans)
                ansSum += x;

            cout << ansSum << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
