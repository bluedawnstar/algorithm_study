#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "minStack.h"

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

void testMinStack() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- min stack ------------------------" << endl;
    {
        int T = 1000;

        MinStack<int> stack;

        for (int i = 0; i < T; i++) {
            stack.push(RandInt32::get());
            assert(stack.min() == *min_element(stack.stack.begin(), stack.stack.end()));
        }
        while (!stack.empty()) {
            assert(stack.min() == *min_element(stack.stack.begin(), stack.stack.end()));
            stack.pop();
        }
    }

    cout << "OK!" << endl;
}
