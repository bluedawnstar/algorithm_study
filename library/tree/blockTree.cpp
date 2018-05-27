#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "blockTree.h"

// http://codeforces.com/blog/entry/46843

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testBlockTree() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Block-Tree (Sqrt-Tree) -------------------" << endl;
    {
        int N = 1000;
        auto tree = makeBlockTree(N, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; }, 0);

        //TODO: tree.addEdge(u, v);
        // ...

        tree.build();

        //TODO: tree.updateLevel(level, value);
        //TODO: tree.querySubtree(node);
    }

    cout << "OK!" << endl;
}
