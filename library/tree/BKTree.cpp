#include <string>
#include <functional>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "BKTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int minEditDistance(const string& v1, const string& v2) {
    int n = int(v1.size());
    int m = int(v2.size());
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    for (int i = 1; i <= m; i++)
        dp[0][i] = i;
    for (int i = 1; i <= n; i++)
        dp[i][0] = i;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (v1[i - 1] == v2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1])) + 1;
        }
    }

    return dp[n][m];
}

void testBKTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- BK-Tree -------------------------------" << endl;
    {
        // dictionary words
        vector<string> dictionary{
            "hell", "help", "shell", "smell", "fell", "felt", "oops", "pop", "oouch", "halt"
        };

        BKTree tree(minEditDistance);

        for (auto& it : dictionary)
            tree.add(it);

        string s1 = "ops";
        auto ans1 = tree.searchSimilarWords(s1);
        auto gt1 = vector<string>{ "oops", "pop" };
        if (ans1 != gt1) {
            cout << "Mismatched : key = \"" << s1 << "\", ans = " << ans1 << ", gt = " << gt1 << endl;
        }
        assert(ans1 == gt1);

        string s2 = "helt";
        auto ans2 = tree.searchSimilarWords(s2);
        auto gt2 = vector<string>{ "hell", "help", "shell", "fell", "felt", "halt" };
        if (ans2 != gt2) {
            cout << "Mismatched : key = \"" << s1 << "\", ans = " << ans1 << ", gt = " << gt1 << endl;
        }
        assert(ans2 == gt2);
    }

    cout << "OK!" << endl;
}
