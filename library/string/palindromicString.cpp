#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "palindromicString.h"

// https://leetcode.com/problems/find-the-closest-palindrome/#/description


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPalindromicString() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- The number of palindromic substrings ---" << endl;
    cout << countPalindromicSubstring("a", 1) << endl;
    cout << countPalindromicSubstring("aa", 2) << endl;
    cout << countPalindromicSubstring("aaa", 3) << endl;
    cout << countPalindromicSubstring("aba", 3) << endl;
    cout << countPalindromicSubstring("abc", 3) << endl;
    cout << countPalindromicSubstring("abcca", 5) << endl;
    cout << countPalindromicSubstring("abca", 4) << endl;

    cout << "-- Length of longest palindromic substring ---" << endl;
    cout << lengthOfLongestPalindromicSubstring("a", 1) << endl;
    cout << lengthOfLongestPalindromicSubstring("aa", 2) << endl;
    cout << lengthOfLongestPalindromicSubstring("aaa", 3) << endl;
    cout << lengthOfLongestPalindromicSubstring("aba", 3) << endl;
    cout << lengthOfLongestPalindromicSubstring("abc", 3) << endl;
    cout << lengthOfLongestPalindromicSubstring("abcca", 5) << endl;
    cout << lengthOfLongestPalindromicSubstring("abca", 4) << endl;

    cout << "-- Longest palindromic substring ---" << endl;
    cout << longestPalindromicSubstring("a", 1) << endl;
    cout << longestPalindromicSubstring("aa", 2) << endl;
    cout << longestPalindromicSubstring("aaa", 3) << endl;
    cout << longestPalindromicSubstring("aba", 3) << endl;
    cout << longestPalindromicSubstring("abc", 3) << endl;
    cout << longestPalindromicSubstring("abcca", 5) << endl;
    cout << longestPalindromicSubstring("abca", 4) << endl;

    cout << "-- Fast longest palindromic substring ---" << endl;
    cout << fastLongestPalindromicSubstring("a", 1) << endl;
    cout << fastLongestPalindromicSubstring("aa", 2) << endl;
    cout << fastLongestPalindromicSubstring("aaa", 3) << endl;
    cout << fastLongestPalindromicSubstring("aba", 3) << endl;
    cout << fastLongestPalindromicSubstring("abc", 3) << endl;
    cout << fastLongestPalindromicSubstring("abcca", 5) << endl;
    cout << fastLongestPalindromicSubstring("abca", 4) << endl;

#define SS(s)   s, sizeof(s) - 1
    cout << fastLongestPalindromicSubstring(SS("babcbabcbaccba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abaaba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abababa")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abcbabcbabcba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("forgeeksskeegfor")) << endl;
    cout << fastLongestPalindromicSubstring(SS("caba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abacdfgdcaba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abacdfgdcabba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abacdedcaba")) << endl;

    cout << "-- PalindromicNumber ---" << endl;
    cout << PalindromicNumber::nearestPalindromic("10") << endl;
    cout << PalindromicNumber::nearestPalindromic("100") << endl;
}
