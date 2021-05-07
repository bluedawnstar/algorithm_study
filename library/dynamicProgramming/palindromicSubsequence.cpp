#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "palindromicSubsequence.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPalindromicSubsequence() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Palindromic Subsequence ------------------------" << endl;

    // the number of palindromic subsequences
    assert(countPalindromicSubsequence("a") == 1);
    assert(countPalindromicSubsequence("aa") == 3);
    assert(countPalindromicSubsequence("aaa") == 7);
    assert(countPalindromicSubsequence("aba") == 5);
    assert(countPalindromicSubsequence("abc") == 3);
    assert(countPalindromicSubsequence("abcca") == 11);

    // length of longest palindromic subsequences
    assert(lengthOfLongestPalindromicSubsequence("a") == 1);
    assert(lengthOfLongestPalindromicSubsequence("aa") == 2);
    assert(lengthOfLongestPalindromicSubsequence("aaa") == 3);
    assert(lengthOfLongestPalindromicSubsequence("aba") == 3);
    assert(lengthOfLongestPalindromicSubsequence("abc") == 1);
    assert(lengthOfLongestPalindromicSubsequence("abcca") == 4);

    // longest palindromic subsequences
    assert(longestPalindromicSubsequence("a") == "a");
    assert(longestPalindromicSubsequence("aa") == "aa");
    assert(longestPalindromicSubsequence("aaa") == "aaa");
    assert(longestPalindromicSubsequence("aba") == "aba");
    assert(longestPalindromicSubsequence("abc") == "c");
    assert(longestPalindromicSubsequence("abcca") == "acca");

    cout << "OK!" << endl;
}
