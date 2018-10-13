#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "schieberVishkinRMQ.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "offlineRMQ.h"
#include "segmentTreeRMQ.h"
#include "sparseTableMin.h"

static int getMin(const vector<int>& v, int L, int R) {
    int res = v[L];
    while (++L <= R) {
        if (res > v[L])
            res = v[L];
    }
    return res;
}

void testSchieberVishkinRMQ() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Schieber-Vishkin RMQ ------------------------" << endl;
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

        SchieberVishkinRMQ<int> rmq(value);
        for (int i = 0; i < T; i++) {
            int ans = value[rmq.queryIndex(query[i].first, query[i].second)];
            int gt = getMin(value, query[i].first, query[i].second);
            if (ans != gt)
                cout << "Mismatch at " << i << ": " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "*** Speed test ***" << endl;
    // sparse table > Schieber-Vishkin > RMQ > Offline RMQ
    {
        int N = 10000;
        int T = 10000;
#ifndef _DEBUG
        N = 1000000;
        T = 10000000;
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
            SparseTableMin rmq(value);

            int ansSum = 0;
            for (auto q : query)
                ansSum += rmq.query(q.first, q.second);

            cout << ansSum << endl;
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        {
            OfflineRMQ<int> rmq;
            auto ans = rmq.solve(value, query);

            int ansSum = 0;
            for (auto x : ans)
                ansSum += x;

            cout << ansSum << endl;
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        {
            SchieberVishkinRMQ<int> rmq(value);

            int ansSum = 0;
            for (auto q : query)
                ansSum += value[rmq.queryIndex(q.first, q.second)];

            cout << ansSum << endl;
        }
        PROFILE_STOP(3);
    }

    cout << "OK!" << endl;
}
