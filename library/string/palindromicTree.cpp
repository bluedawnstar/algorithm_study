#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "palindromicTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "palindromicSubstringDP.h"
#include "palindromicSubstringLongest_manacher.h"

static bool isPalindrome(const string& s, int L, int R) {
    while (L < R) {
        if (s[L++] != s[R--])
            return false;
    }
    return true;
}

static vector<string> findSuffixPalindromes(const string& s, int R) {
    vector<string> res;
    for (int i = R; i >= 0; i--) {
        if (isPalindrome(s, i, R))
            res.push_back(s.substr(i, R - i + 1));
    }
    return res;
}

void testPalindromicTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Palindromic Tree ---" << endl;
    {
        vector<string> in{
            "a",
            "aa",
            "aaa",
            "aba",
            "abc",
            "abcca",
            "abca"
        };

        for (auto s : in) {
            PalindromicTree tree(10);

            int gt = PalindromicSubstringDP::countPalindromicSubstring(s);
            int ans = int(tree.count(s));
            if (ans != gt) {
                cout << "gt = " << gt << ", ans = " << ans << endl;
            }
            assert(ans == gt);
        }

        for (int i = 0; i < 100; i++) {
            string s;
            for (int j = 0; j < 100; j++)
                s += 'a' + RandInt32::get() % 3;

            // the number of palindromic strings
            {
                vector<int> gt(int(s.length()));
                int prevN = 0;
                for (int i = 0; i < int(s.length()); i++) {
                    int currN = PalindromicSubstringDP::countPalindromicSubstring(s.c_str(), i + 1);
                    gt[i] = currN - prevN;
                    prevN = currN;
                }

                PalindromicTree tree(int(s.length()));
                vector<int> ans = tree.countAll(s);
                if (ans != gt) {
                    cout << s << endl;
                    cout << "gt = " << gt << ", ans = " << ans << endl;
                }
                assert(ans == gt);
            }
            // lengths of palindromic strings
            {
                vector<int> gt = LongestPalindromicSubstring::getLongestPalindromesByEnd(s);

                PalindromicTree tree(int(s.length()));
                vector<int> ans;
                for (int i = 0; i < int(s.length()); i++) {
                    tree.extend(s[i]);
                    ans.push_back(tree.nodes[tree.lastSuffix].len);
                }
                if (ans != gt) {
                    cout << s << endl;
                    cout << "gt = " << gt << ", ans = " << ans << endl;
                }
                assert(ans == gt);
            }
        }
    }
    {
        const string s = "ababaacabd";

        PalindromicTree tree(int(s.length()));
        for (int i = 0; i < int(s.length()); i++) {
            tree.extend(s[i]);

            vector<string> pals = tree.getLastSuffixPalindromes();
            reverse(pals.begin(), pals.end());

            vector<string> gt = findSuffixPalindromes(s, i);

            if (pals != gt)
                cout << "Mismatched at " << i << ": " << pals << endl;
            assert(pals == gt);
            assert(pals.size() == tree.nodes[tree.lastSuffix].count);
        }
    }

    cout << "OK!" << endl;
}
