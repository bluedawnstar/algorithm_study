#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "palindromicSubsequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPalindromicSubsequence() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- The number of palindromic subsequences ---" << endl;
    cout << countPalindromicSubsequence("a", 1) << endl;
    cout << countPalindromicSubsequence("aa", 2) << endl;
    cout << countPalindromicSubsequence("aaa", 3) << endl;
    cout << countPalindromicSubsequence("aba", 3) << endl;
    cout << countPalindromicSubsequence("abc", 3) << endl;
    cout << countPalindromicSubsequence("abcca", 5) << endl;

    cout << "-- Length of longest palindromic subsequence ---" << endl;
    cout << lengthOfLongestPalindromicSubsequence("a", 1) << endl;
    cout << lengthOfLongestPalindromicSubsequence("aa", 2) << endl;
    cout << lengthOfLongestPalindromicSubsequence("aaa", 3) << endl;
    cout << lengthOfLongestPalindromicSubsequence("aba", 3) << endl;
    cout << lengthOfLongestPalindromicSubsequence("abc", 3) << endl;
    cout << lengthOfLongestPalindromicSubsequence("abcca", 5) << endl;

    cout << "-- Longest palindromic subsequence ---" << endl;
    cout << longestPalindromicSubsequence("a", 1) << endl;
    cout << longestPalindromicSubsequence("aa", 2) << endl;
    cout << longestPalindromicSubsequence("aaa", 3) << endl;
    cout << longestPalindromicSubsequence("aba", 3) << endl;
    cout << longestPalindromicSubsequence("abc", 3) << endl;
    cout << longestPalindromicSubsequence("abcca", 5) << endl;
}
