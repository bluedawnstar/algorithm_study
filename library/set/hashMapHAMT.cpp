#include <memory>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "hashMapHAMT.h"

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

static string makeRandomString(int len) {
    string res(len, ' ');
    for (int i = 0; i < len; i++)
        res[i] = RandInt32::get() % 26 + 'a';
    return res;
}

static unsigned int getHash(const string& s) {
    unsigned int h = 0;
    for (int i = 0; i < (int)s.length(); i++)
        h = h * 31 + (s[i] - 'a');
    return h;
}

template <typename KeyT, typename ValueT>
struct SqrtMap {
    static const int BIT = 20;
    static const int MASK = 0xfffff;

    vector<map<KeyT, ValueT>> maps;

    SqrtMap() : maps(1 << BIT) {
    }

    typename map<KeyT, ValueT>::mapped_type& operator[](const string& key) {
        return maps[getHash(key) & MASK][key];
    }

    typename map<KeyT, ValueT>::mapped_type& operator[](unsigned long long key) {
        return maps[key & MASK][key];
    }
};

void testHashMapHAMT() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Hash Map with HAMT -----------------------------" << endl;
    {
        int N = 1000000;
        int T = 10000;
        //int N = 10;
        //int T = 100;

        vector<int> gt(N, 0);
        HashMapHAMT<int, int> shm([](int index) { return (unsigned int)index; }, 0);
        SqrtMap<int, int> sqrtm;

        vector<int> inIdx(T);
        vector<int> inValue(T);
        for (int i = 0; i < T; i++) {
            int idx = RandInt32::get() % N;
            int value = RandInt32::get();

            inIdx[i] = idx;
            inValue[i] = value;

            //shm[idx] = value;
            shm.set(idx, value);
            if (shm.get(idx) != value) {
                cout << "[" << i << "] " << shm.get(idx) << ", " << value << endl;
            }

            sqrtm[idx] = value;
            gt[idx] = value;
        }

        for (int i = 0; i < N; i++) {
            if (shm.get(i) != gt[i]) {
                cout << "[" << i << "] " << shm.get(i) << ", " << gt[i] << endl;
            }
            assert(shm.get(i) == gt[i]);
            assert(sqrtm[i] == gt[i]);
        }
    }
    cout << "Speed test with long long : HAMT Hash Map vs. unordered_map" << endl;
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
            //HashMapHAMT<long long, int, HAMTConfigUInt32Depth1> shm([](long long index) { return (unsigned int)index; });
            //HashMapHAMT<long long, int, HAMTConfigUInt64<2,25,128>> shm([](long long index) { return (unsigned int)index & 0x01ffffffu; });
            HashMapHAMT<long long, int, HAMTConfigUInt64<1, 32, 128>> shm([](long long index) { return (unsigned int)index; });
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
    cout << "Speed test with long long : HAMT Hash Map vs. unordered_map" << endl;
    {
        int N = 10000000;

        vector<pair<string, int>> in(N);
        for (int i = 0; i < N; i++) {
            in[i].first = makeRandomString(20);
            in[i].second = RandInt32::get();
        }
        {
            PROFILE_START(0);
            unordered_map<string, int> M;
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
            HashMapHAMT<string, int, HAMTConfigUInt64<1, 32, 32>> shm(getHash);
            for (int i = 0; i < N; i++)
                //shm[in[i].first] = in[i].second;
                shm.set(in[i].first, in[i].second);
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
            SqrtMap<string, int> sqrtm;
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
            map<string, int> m;
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
