#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "palindromicSubstringDP.h"
#include "palindromicSubstringLongest_manacher.h"
#include "palindromicSubstringAll_manacher.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPalindromicSubstring() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- The number of palindromic substrings ---" << endl;
    cout << PalindromicSubstringDP::countPalindromicSubstring("a", 1) << endl;
    cout << PalindromicSubstringDP::countPalindromicSubstring("aa", 2) << endl;
    cout << PalindromicSubstringDP::countPalindromicSubstring("aaa", 3) << endl;
    cout << PalindromicSubstringDP::countPalindromicSubstring("aba", 3) << endl;
    cout << PalindromicSubstringDP::countPalindromicSubstring("abc", 3) << endl;
    cout << PalindromicSubstringDP::countPalindromicSubstring("abcca", 5) << endl;
    cout << PalindromicSubstringDP::countPalindromicSubstring("abca", 4) << endl;

    cout << "-- Length of longest palindromic substring ---" << endl;
    cout << PalindromicSubstringDP::lengthOfLongestPalindromicSubstring("a", 1) << endl;
    cout << PalindromicSubstringDP::lengthOfLongestPalindromicSubstring("aa", 2) << endl;
    cout << PalindromicSubstringDP::lengthOfLongestPalindromicSubstring("aaa", 3) << endl;
    cout << PalindromicSubstringDP::lengthOfLongestPalindromicSubstring("aba", 3) << endl;
    cout << PalindromicSubstringDP::lengthOfLongestPalindromicSubstring("abc", 3) << endl;
    cout << PalindromicSubstringDP::lengthOfLongestPalindromicSubstring("abcca", 5) << endl;
    cout << PalindromicSubstringDP::lengthOfLongestPalindromicSubstring("abca", 4) << endl;

    cout << "-- Longest palindromic substring ---" << endl;
    cout << PalindromicSubstringDP::longestPalindromicSubstring("a", 1) << endl;
    cout << PalindromicSubstringDP::longestPalindromicSubstring("aa", 2) << endl;
    cout << PalindromicSubstringDP::longestPalindromicSubstring("aaa", 3) << endl;
    cout << PalindromicSubstringDP::longestPalindromicSubstring("aba", 3) << endl;
    cout << PalindromicSubstringDP::longestPalindromicSubstring("abc", 3) << endl;
    cout << PalindromicSubstringDP::longestPalindromicSubstring("abcca", 5) << endl;
    cout << PalindromicSubstringDP::longestPalindromicSubstring("abca", 4) << endl;

    cout << "-- Fast longest palindromic substring ---" << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring("a", 1) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring("aa", 2) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring("aaa", 3) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring("aba", 3) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring("abc", 3) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring("abcca", 5) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring("abca", 4) << endl;

#define SS(s)   s, sizeof(s) - 1
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("babcbabcbaccba")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("abaaba")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("abababa")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("abcbabcbabcba")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("forgeeksskeegfor")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("caba")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("abacdfgdcaba")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("abacdfgdcabba")) << endl;
    cout << LongestPalindromicSubstring::longestPalindromicSubstring(SS("abacdedcaba")) << endl;
}
