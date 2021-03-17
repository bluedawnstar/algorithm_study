#include <functional>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "treeDP_topologicalOrderCount.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testTopologicalOrderOnDAGLikeReversedTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- The number of topological order on DAG like reversed tree -----------------" << endl;
    {
        const int N = 4;
        DAGLikeReversedTree<> dagTree(N);
        dagTree.add(0, 1);
        dagTree.add(1, 2);
        dagTree.add(1, 3);
        dagTree.build(0);

        vector<int> order = dagTree.relativeOrderByTopologicalOrderCount();
        vector<int> cnt = dagTree.countTopologicalOrder();
        assert(order[N - 2] == 3);
        assert(cnt[order[N - 2]] == 2);
    }

    cout << "OK!" << endl;
}
