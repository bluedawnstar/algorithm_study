#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "sparseTableSimpleRMQ.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "segmentTreeRMQ.h"
#include "sparseTableMin.h"
#include "schieberVishkinRMQ.h"

static int getMin(const vector<int>& v, int L, int R) {
    int res = v[L];
    while (++L <= R) {
        if (res > v[L])
            res = v[L];
    }
    return res;
}

void testSparseTableSimpleRMQ() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Sparse Table RMQ ------------------------" << endl;
    {
        int N = 10000;
        int T = 10000;
#ifdef _DEBUG
        N = 1000;
        T = 1000;
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

        SimpleSparseTableRMQ<int> rmq(value);
        for (int i = 0; i < T; i++) {
            int gt = getMin(value, query[i].first, query[i].second);
            int ans = rmq.query(query[i].first, query[i].second);
            if (ans != gt)
                cout << "Mismatch at " << i << ": " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 1024 * 1024;
        int T = 10000;
#ifdef _DEBUG
        N = 8 * 1024;
        T = 1000;
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

        SimpleSparseTableRMQ<int> rmq(value);
        for (int i = 0; i < T; i++) {
            int gt = getMin(value, query[i].first, query[i].second);
            int ans = rmq.query(query[i].first, query[i].second);
            if (ans != gt)
                cout << "Mismatch at " << i << ": " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "*** Speed test ***" << endl;
    {
        int N = 10000;
        int T = 10000;
#ifndef _DEBUG
        N = 10000000;
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
            SparseTableMin rmq;
            rmq.build(value);

            int ansSum = 0;
            for (auto q : query)
                ansSum += rmq.query(q.first, q.second);

            cout << ansSum << endl;
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        {
            SchieberVishkinRMQ<int> rmq;
            rmq.build(value);

            int ansSum = 0;
            for (auto q : query)
                ansSum += value[rmq.queryIndex(q.first, q.second)];

            cout << ansSum << endl;
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        {
            SimpleSparseTableRMQ<int> rmq(value);

            int ansSum = 0;
            for (auto q : query)
                ansSum += rmq.query(q.first, q.second);

            cout << ansSum << endl;
        }
        PROFILE_STOP(3);
    }

    cout << "OK!" << endl;
}
