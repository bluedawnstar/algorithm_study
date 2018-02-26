#include <vector>
#include <algorithm>

using namespace std;

#include "persistentStack.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPersistentStack() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Persistent Stack ----------" << endl;

    int N = 1000;
    int T = 1000;
    vector<int> st;

    PersistentStack<int> ps(N);

    vector<tuple<int, int, int>> gt; // (time, size, top)
    for (int i = 0; i < T; i++) {
        int t = rand() % 4;
        if (t == 0 && !st.empty()) {
            st.pop_back();
            int size = (int)st.size();
            int time = ps.pop();
            gt.emplace_back(time, size, size > 0 ? st.back() : -1);
        } else {
            int x = rand();
            st.push_back(x);
            int size = (int)st.size();
            int time = ps.push(x);
            gt.emplace_back(time, size, x);
        }
    }

    int time, size, top;
    for (auto& it : gt) {
        tie(time, size, top) = it;
        assert(ps.size(time) == size);
        if (size > 0)
            assert(ps.top(time) == top);
    }

    cout << "OK!" << endl;
}
