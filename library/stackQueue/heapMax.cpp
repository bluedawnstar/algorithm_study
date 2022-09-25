#include <vector>
#include <algorithm>

using namespace std;

#include "heapMax.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
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

    cout << "OK!" << endl;
}
