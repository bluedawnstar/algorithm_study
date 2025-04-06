#include <vector>
#include <queue>
#include <functional>

using namespace std;

#include "heapLazy.h"

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

void testHeapLazy() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- lazy heap ------------------------" << endl;
    {
        const int T = 1000;
        const int N = 1000;

        vector<int> in(N);
        for (int t = 0; t < T; t++) {
            for (int i = 0; i < N; i++)
                in[i] = RandInt32::get();

            HeapLazy<int> heap;
            for (auto x : in)
                heap.push(x);
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

            HeapLazy<int> heap;
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

            HeapLazy<int> heap1;
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

            sort(in.begin(), in.end());
            vector<int> gt(in.begin(), in.begin() + K);
            if (out1 != gt) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(out1 == gt);
        }
    }
    {
        const int T = 1000;
        const int N = 1000;
        const int K = 100;

        vector<vector<int>> in(T, vector<int>(N));
        for (int t = 0; t < T; t++) {
            for (int i = 0; i < N; i++)
                in[t][i] = RandInt32::get();
        }

        PROFILE_START(push_pop);
        for (int t = 0; t < T; t++) {
            HeapLazy<int> heap;
            for (int i = 0; i < N; i++) {
                heap.push(in[t][i]);
                if (heap.size() > K)
                    heap.pop();
            }
            if (heap.size() != K)
                cout << "Mismatched!" << endl;
        }
        PROFILE_STOP(push_pop);

        PROFILE_START(stl);
        for (int t = 0; t < T; t++) {
            priority_queue<int> heap;
            for (int i = 0; i < N; i++) {
                heap.push(in[t][i]);
                if (heap.size() > K)
                    heap.pop();
            }
            if (heap.size() != K)
                cout << "Mismatched!" << endl;
        }
        PROFILE_STOP(stl);
    }
    cout << "OK!" << endl;
}
