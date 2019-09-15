#include <vector>
#include <string>

using namespace std;

#include "reverseSubstringsBetweenParentheses.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testReverseSubstringsBetweenParentheses() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Reverse Substrings Between Parentheses ---------" << endl;
    {
        vector<pair<string, string>> test{
            { "(abcde)", "edcba" },
            { "(a(bc)de)", "edbca" },
            { "a(bc)de", "acbde" },
            { "a(bcdef(ghi)jk)lm", "akjghifedcblm" },
            { "(a(bcdef(ghi)jk)lm)", "mlbcdefihgjka" },
        };
        for (auto& t : test) {
            auto ans1 = ReverseSubstringsBetweenParentheses::reverseParenthesesNaive(t.first);
            auto ans2 = ReverseSubstringsBetweenParentheses::reverseParentheses(t.first);
            if (ans1 != t.second || ans2 != t.second)
                cout << "Mismatched: " << t << " => " << ans1 << ", " << ans2 << endl;
            assert(ans1 == t.second && ans2 == t.second);
        }
    }

    cout << "OK!" << endl;
}
