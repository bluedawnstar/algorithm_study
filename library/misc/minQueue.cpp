#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "minQueue.h"

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

void testMinQueue() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- min queue ------------------------" << endl;
    {
        int T = 1000;

        MinQueue<int> queue;

        for (int i = 0; i < T; i++) {
            queue.push(RandInt32::get());
            assert(queue.min() == *min_element(queue.queue.begin(), queue.queue.end()));
        }
        while (!queue.empty()) {
            assert(queue.min() == *min_element(queue.queue.begin(), queue.queue.end()));
            queue.pop();
        }
    }
    {
        int T = 1000;

        MinQueue<int> queue;
        MinQueue2<int> queue2;

        for (int i = 0; i < T; i++) {
            auto x = RandInt32::get();
            queue.push(x);
            queue2.push(x);
            assert(queue.min() == queue2.min());
        }
        while (!queue2.empty()) {
            assert(!queue.empty());
            assert(queue.min() == queue2.min());
            queue.pop();
            queue2.pop();
        }
    }
    {
        int T = 1000;

        MinQueue<int> queue;
        MinQueue3<int> queue3;

        for (int i = 0; i < T; i++) {
            auto x = RandInt32::get();
            queue.push(x);
            queue3.push(x);
            assert(queue.min() == queue3.min());
            assert(queue.front() == queue3.front());
        }
        while (!queue3.empty()) {
            assert(!queue.empty());
            assert(queue.min() == queue3.min());
            assert(queue.front() == queue3.front());
            queue.pop();
            queue3.pop();
        }
    }

    cout << "OK!" << endl;
}
