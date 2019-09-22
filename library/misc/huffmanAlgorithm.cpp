#include <queue>
#include <vector>
#include <functional>

using namespace std;

#include "huffmanAlgorithm.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testHuffmanAlgorithm() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Huffman's algorithm ------------------------" << endl;
    {
        vector<int> cost{ 1, 2 };
        int splitCost = 5;
        int gt = 7;

        auto ans1 = BlockBuilder::minBuildTime(cost, splitCost);
        auto ans2 = BlockBuilder::minBuildTimeWithBinarySearch(cost, splitCost);
        if (ans1 != gt || ans2 != gt)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ans1 == gt && ans2 == gt);
    }
    {
        vector<int> cost{ 1, 2, 3 };
        int splitCost = 1;
        int gt = 4;

        auto ans1 = BlockBuilder::minBuildTime(cost, splitCost);
        auto ans2 = BlockBuilder::minBuildTimeWithBinarySearch(cost, splitCost);
        if (ans1 != gt || ans2 != gt)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ans1 == gt && ans2 == gt);
    }
    cout << "OK!" << endl;
}
