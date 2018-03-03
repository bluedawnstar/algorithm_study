#include <cmath>
#include <vector>
#include <functional>
#include <set>
#include <algorithm>

using namespace std;

#include "dynamicUpperEnvelope.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testDynamicUpperEnvelope() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Dynamic Upper Envelope (convex hull for maximum) -----------" << endl;
    {
        DynamicUpperEnvelope envelope;
        envelope.add(1, 0);
        envelope.add(-1, 5);

        vector<long long> gt{ 5, 4, 3, 3, 4, 5 };
        vector<long long> ans(6);
        for (int x = 0; x <= 5; x++)
            ans[x] = envelope.query(x);
        assert(gt == ans);
    }
    {
        DynamicUpperEnvelope envelope;
        envelope.add(-1, 5);
        envelope.add(1, 0);

        vector<long long> gt{ 5, 4, 3, 3, 4, 5 };
        vector<long long> ans(6);
        for (int x = 0; x <= 5; x++)
            ans[x] = envelope.query(x);
        assert(gt == ans);
    }
    {
        int N = 100;
        int T = 200;
        int L = -1000, R = 1000;

        vector<DynamicUpperEnvelope::Line> lines;
        for (int i = 0; i < N; i++)
            lines.push_back(DynamicUpperEnvelope::Line{ (long long)rand() - RAND_MAX / 2, (long long)rand() - RAND_MAX / 2 });

        DynamicUpperEnvelope envelope;
        for (int i = 0; i < N; i++)
            envelope.add(lines[i].m, lines[i].b);

        vector<long long> gt(T, -envelope.INF);
        vector<long long> ans(T);
        for (int i = 0; i < T; i++) {
            for (int j = 0; j < N; j++)
                gt[i] = max(gt[i], lines[j].get(i));
            ans[i] = envelope.query(i);
        }

        assert(gt == ans);
    }

    cout << "OK!" << endl;
}
