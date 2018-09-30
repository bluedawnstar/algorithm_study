#include <vector>

using namespace std;

#include "partitionProblem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPartitionProblem() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Partition Problem -----------------------------" << endl;
    {
        vector<int> value{ 3, 1, 1, 2, 2, 2 };

        PartitionProblemEx dp(value);

        int cnt = dp.getPartitionCount();
        cout << "Partition count = " << cnt << endl;

        auto part = dp.getAnyPartition();
        cout << "A partition = " << part.first << ", " << part.second << endl;

        assert(cnt == 0);
    }
    {
        vector<int> value{ 3, 1, 1, 2, 2, 1 };

        PartitionProblemEx dp(value);

        int cnt = dp.getPartitionCount();
        cout << "Partition count = " << cnt << endl;

        auto part = dp.getAnyPartition();
        cout << "A partition = " << part.first << ", " << part.second << endl;

        assert(cnt == 5);
    }
    {
        vector<int> value{ 1, 1, 1, 1 };

        PartitionProblemEx dp(value);

        int cnt = dp.getPartitionCount();
        cout << "Partition count = " << cnt << endl;

        auto part = dp.getAnyPartition();
        cout << "A partition = " << part.first << ", " << part.second << endl;

        assert(cnt == 3);
    }

    cout << "OK!" << endl;
}




