#include <memory>
#include <map>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

#include "simpleHashMap.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include <unordered_map>

void testSimpleHashMap() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sparse Array --------------------" << endl;
    {
        int T = 10000;
        int N = 1000000;

        vector<int> gt(N);
        SimpleHashMap<int, int> sa(4);

        for (int i = 0; i < T; i++) {
            int idx = RandInt32::get() % N;
            int value = RandInt32::get();
            sa[idx] = value;
            //sa.set(idx, value);
            gt[idx] = value;
        }

        for (int i = 0; i < T; i++) {
            assert(sa[i] == gt[i]);
        }
    }
    cout << "Speed test : Sparse Array vs. unordered_map" << endl;
    {
        int N = 10000000;

        vector<pair<long long, int>> in(N);
        for (int i = 0; i < N; i++) {
            in[i].first = RandInt64::get();
            in[i].second = RandInt32::get();
        }
        {
            PROFILE_START(0);
            unordered_map<long long, int> M;
            for (int i = 0; i < N; i++)
                M[in[i].first] = in[i].second;
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < N; i++) {
                if (M[in[i].first] != in[i].second)
                    cout << "ERROR!" << endl;;
            }
            PROFILE_STOP(1);
        }
        {
            PROFILE_START(2);
            SimpleHashMap<long long, int> sa;
            for (int i = 0; i < N; i++)
                sa[in[i].first] = in[i].second;
                //sa.set(in[i].first, in[i].second);
            PROFILE_STOP(2);

            PROFILE_START(3);
            for (int i = 0; i < N; i++) {
                //if (sa[in[i].first] != in[i].second)
                if (sa.get(in[i].first) != in[i].second)
                    cout << "ERROR!" << endl;
            }
            PROFILE_STOP(3);
        }
    }

    cout << "OK!" << endl;
}
