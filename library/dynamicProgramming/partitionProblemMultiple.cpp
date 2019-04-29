#include <vector>
#include <algorithm>

using namespace std;

#include "partitionProblemMultiple.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testPartitionProblemMultiple() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Multiple Partition Problem -----------------------------" << endl;
    {
        vector<int> value{ 3, 5, 1, 2, 6 };

        auto ans = MultiplePartitionProblem::countPartitions(value, 3, 12);
        assert(ans == 8);
    }

    cout << "OK!" << endl;
}
