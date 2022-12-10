#include <vector>
#include <algorithm>

using namespace std;

#include "heapMax.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <queue>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testMaxHeap() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- max heap ------------------------" << endl;
    {
        const int T = 1000;
        const int N = 1000;

        vector<int> in(N);
        for (int t = 0; t < T; t++) {
            for (int i = 0; i < N; i++)
                in[i] = RandInt32::get();

            MaxHeap<int> heap(in.data(), int(in.size()));
            vector<int> out;
            while (!heap.empty()) {
                out.push_back(heap.top());
                heap.pop();
            }
            reverse(out.begin(), out.end());

            sort(in.begin(), in.end());
            if (in != out) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(in == out);
        }
    }
    {
        const int T = 1000;
        const int N = 1000;

        vector<int> in(N);
        for (int t = 0; t < T; t++) {
            for (int i = 0; i < N; i++)
                in[i] = RandInt32::get();

            MaxHeap<int> heap;
            for (int i = 0; i < N; i++)
                heap.push(in[i]);

            vector<int> out;
            while (!heap.empty()) {
                out.push_back(heap.top());
                heap.pop();
            }
            reverse(out.begin(), out.end());

            sort(in.begin(), in.end());
            if (in != out) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(in == out);
        }
    }
    {
        const int T = 1000;
        const int N = 1000;
        const int K = 100;

        vector<int> in(N);
        for (int t = 0; t < T; t++) {
            for (int i = 0; i < N; i++)
                in[i] = RandInt32::get();

            MaxHeap<int> heap1;
            for (int i = 0; i < N; i++) {
                heap1.push(in[i]);
                if (heap1.size() > K)
                    heap1.pop();
            }
            vector<int> out1;
            while (!heap1.empty()) {
                out1.push_back(heap1.top());
                heap1.pop();
            }
            reverse(out1.begin(), out1.end());

            //---

            MaxHeap<int> heap2;
            for (int i = 0; i < N; i++) {
                heap2.push(K, in[i]);
            }
            vector<int> out2;
            while (!heap2.empty()) {
                out2.push_back(heap2.top());
                heap2.pop();
            }
            reverse(out2.begin(), out2.end());

            sort(in.begin(), in.end());
            vector<int> gt(in.begin(), in.begin() + K);
            if (out1 != gt) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            if (out2 != gt) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(out1 == gt && out2 == gt);
        }
    }
    {
        const int T = 1000;
        const int N = 10000;
        const int K = 100;

        vector<vector<int>> in(T, vector<int>(N));
        for (int t = 0; t < T; t++) {
            for (int i = 0; i < N; i++)
                in[t][i] = RandInt32::get();
        }

        PROFILE_START(push_pop_1);
        for (int t = 0; t < T; t++) {
            MaxHeap<int> heap2;
            for (int i = 0; i < N; i++) {
                heap2.push(K, in[t][i]);
            }
            if (heap2.size() != K)
                cout << "Mismatched!" << endl;
        }
        PROFILE_STOP(push_pop_1);

        PROFILE_START(push_pop_2);
        for (int t = 0; t < T; t++) {
            MaxHeap<int> heap1;
            for (int i = 0; i < N; i++) {
                heap1.push(in[t][i]);
                if (heap1.size() > K)
                    heap1.pop();
            }
            if (heap1.size() != K)
                cout << "Mismatched!" << endl;
        }
        PROFILE_STOP(push_pop_2);

        PROFILE_START(stl_3);
        for (int t = 0; t < T; t++) {
            priority_queue<int> heap3;
            for (int i = 0; i < N; i++) {
                heap3.push(in[t][i]);
                if (heap3.size() > K)
                    heap3.pop();
            }
            if (heap3.size() != K)
                cout << "Mismatched!" << endl;
        }
        PROFILE_STOP(stl_3);
    }
    cout << "OK!" << endl;
}
