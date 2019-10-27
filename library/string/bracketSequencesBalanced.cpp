#include <string>
#include <stack>
#include <vector>

using namespace std;

#include "bracketSequencesBalanced.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static vector<string> findAll(int n) {
    vector<string> res;
    if (n & 1)
        return res;

    string s(n, '(');
    for (int i = n / 2; i < n; i++)
        s[i] = ')';

    do {
        if (isBalancedSequence(s))
            res.push_back(s);
    } while (next_permutation(s.begin(), s.end()));

    return res;
}

vector<string> findAllAlmost(int n) {
    auto all = findAll(n - 1);

    vector<string> res;
    for (auto& s : all) {
        for (int i = 0; i < n; i++) {
            string t = s;
            t.insert(t.begin() + i, '(');
            res.push_back(t);

            t = s;
            t.insert(t.begin() + i, ')');
            res.push_back(t);
        }
    }
    sort(res.begin(), res.end());
    res.erase(unique(res.begin(), res.end()), res.end());
    return res;
}

void testBracketSequencesBalanced() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Balanced Bracket Sequences -------------------------" << endl;
    {
        for (int N = 2; N <= 20; N += 2) {
            auto gt = findAll(N);

            vector<string> ans;
            BalancedBracketSequences<int> dp;
            dp.prepare(N);
            for (int i = 1; i <= dp.dp[N][0]; i++) {
                auto s = dp.kthBalanced(N, i);
                if (s.empty())
                    break;
                ans.push_back(s);
            }

            if (gt != ans) {
                cout << "Mismatched: N = " << N << endl;
                cout << "--- gt" << endl;
                for (auto& s : gt)
                    cout << s << endl;
                cout << "--- ans" << endl;
                for (auto& s : ans)
                    cout << s << endl;
            }
            assert(gt == ans);
        }
    }
    cout << "OK!" << endl;
    {
        for (int N = 1; N <= 15; N += 2) {
            auto gt = findAllAlmost(N);

            vector<string> ans;
            BalancedBracketSequences<int> dp;
            dp.prepare(N);

            auto count = dp.countAlmostBalanced(N);
            for (int i = 1; i <= count; i++) {
                auto s = dp.kthAlmostBalanced(N, i);
                if (s.empty())
                    break;
                ans.push_back(s);
            }

            if (gt != ans) {
                cout << "Mismatched: N = " << N << endl;
                cout << "--- gt" << endl;
                for (auto& s : gt)
                    cout << s << endl;
                cout << "--- ans" << endl;
                for (auto& s : ans)
                    cout << s << endl;
            }
            assert(gt == ans);
        }
    }
    cout << "OK!" << endl;
}
