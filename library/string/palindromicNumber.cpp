#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "palindromicNumber.h"

// https://leetcode.com/problems/find-the-closest-palindrome/#/description

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPalindromicNumber() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Palindromic Number ---" << endl;
    cout << PalindromicNumber::nearestPalindromic("10") << endl;
    cout << PalindromicNumber::nearestPalindromic("100") << endl;
}
