#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

#include "rangeSet.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testRangeSet() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Range Set ------------------------" << endl;
    {
        int N = 10;
        vector<pair<int, int>> gt;
        RangeSet<int> rangeSet;
        for (int i = 0; i < N; i++) {
            int L = RandInt32::get();
            int R = RandInt32::get();
            if (L > R)
                swap(L, R);
            gt.emplace_back(L, R);
            rangeSet.insert(L, R);
        }

        // merge
        sort(gt.begin(), gt.end());
        int j = 0;
        for (int i = 1; i < int(gt.size()); i++) {
            if (gt[j].second >= gt[i].first)
                gt[j].second = max(gt[j].second, gt[i].second);
            else
                gt[++j] = gt[i];
        }
        gt.resize(j + 1);

        auto ans = rangeSet.get();

        if (gt != ans) {
            cout << "gt = " << gt << endl;
            cout << "ans = " << ans << endl;
        }

        assert(gt == ans);
    }

    cout << "OK!" << endl;
}
