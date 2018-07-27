#include <memory>
#include <functional>
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

#include <map>
#include <unordered_map>

template <typename KeyT, typename ValueT>
struct SqrtMap {
    static const int BIT = 20;
    static const int MASK = 0xfffff;

    vector<map<KeyT, ValueT>> maps;

    SqrtMap() : maps(1 << BIT) {
    }

    typename map<KeyT, ValueT>::mapped_type& operator[](const KeyT& key) {
        return maps[key & MASK][key];
    }
};

void testSimpleHashMap() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Hash Map-----------------------------" << endl;
    {
        int T = 10000;
        int N = 1000000;

        vector<int> gt(N, 0);
        SimpleHashMap<int, int> shm([](int index) { return index; }, 0);
        SqrtMap<int, int> sqrtm;

        for (int i = 0; i < T; i++) {
            int idx = RandInt32::get() % N;
            int value = RandInt32::get();

            shm[idx] = value;
            //shm.set(idx, value);

            sqrtm[idx] = value;

            gt[idx] = value;
        }

        for (int i = 0; i < N; i++) {
            assert(shm[i] == gt[i]);
            assert(sqrtm[i] == gt[i]);
        }
    }
    cout << "Speed test with long long : Simple Hash Map vs. unordered_map" << endl;
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
            SimpleHashMap<long long, int> shm([](long long index) { return int(index); });
            for (int i = 0; i < N; i++)
                shm[in[i].first] = in[i].second;
                //shm.set(in[i].first, in[i].second);
            PROFILE_STOP(2);

            PROFILE_START(3);
            for (int i = 0; i < N; i++) {
                //if (shm[in[i].first] != in[i].second)
                if (shm.get(in[i].first) != in[i].second)
                    cout << "ERROR!" << endl;
            }
            PROFILE_STOP(3);
        }
        {
            PROFILE_START(4);
            SqrtMap<long long, int> sqrtm;
            for (int i = 0; i < N; i++)
                sqrtm[in[i].first] = in[i].second;
            PROFILE_STOP(4);

            PROFILE_START(5);
            for (int i = 0; i < N; i++) {
                if (sqrtm[in[i].first] != in[i].second)
                    cout << "ERROR!" << endl;
            }
            PROFILE_STOP(5);
        }
        {
            PROFILE_START(6);
            map<long long, int> m;
            for (int i = 0; i < N; i++)
                m[in[i].first] = in[i].second;
            PROFILE_STOP(6);

            PROFILE_START(7);
            for (int i = 0; i < N; i++) {
                if (m[in[i].first] != in[i].second)
                    cout << "ERROR!" << endl;
            }
            PROFILE_STOP(7);
        }
    }

    cout << "OK!" << endl;
}
