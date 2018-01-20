#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "palindromicSubstring.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPalindromicSubstring() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Palindromic Substring ------------------------" << endl;

    // the number of palindromic substrings
    assert(countPalindromicSubstring("a") == 1);
    assert(countPalindromicSubstring("aa") == 3);
    assert(countPalindromicSubstring("aaa") == 6);
    assert(countPalindromicSubstring("aba") == 4);
    assert(countPalindromicSubstring("abc") == 3);
    assert(countPalindromicSubstring("abcca") == 6);

    // length of longest palindromic substrings
    assert(lengthOfLongestPalindromicSubstring("a") == 1);
    assert(lengthOfLongestPalindromicSubstring("aa") == 2);
    assert(lengthOfLongestPalindromicSubstring("aaa") == 3);
    assert(lengthOfLongestPalindromicSubstring("aba") == 3);
    assert(lengthOfLongestPalindromicSubstring("abc") == 1);
    assert(lengthOfLongestPalindromicSubstring("abcca") == 2);

    // longest palindromic substrings
    assert(longestPalindromicSubstring("a") == "a");
    assert(longestPalindromicSubstring("aa") == "aa");
    assert(longestPalindromicSubstring("aaa") == "aaa");
    assert(longestPalindromicSubstring("aba") == "aba");
    assert(longestPalindromicSubstring("abc") == "a");
    assert(longestPalindromicSubstring("abcca") == "cc");

    cout << "OK!" << endl;
}
