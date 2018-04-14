#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "searchPrefixFunction.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <iostream>
#include "../common/iostreamhelper.h"

void testSearchPrefixFunction() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- checkRepeatedString() ---------" << endl;            
    cout << checkRepeatedString("abcdefg") << endl;                 // (-1, -1)
    cout << checkRepeatedString("abcabc") << endl;                  // (3, 2)
    cout << checkRepeatedString("abacdacdb") << endl;               // (-1, -1)
    cout << checkRepeatedString("aaaaaaaa") << endl;                // (1, 8)
    cout << checkRepeatedString("abcabcabcabc") << endl;            // (3, 4)
    cout << "-- getMaxRepeatedSubstring() ---------" << endl;
    cout << getMaxRepeatedSubstring("abcdefg") << endl;             // (-1, -1, -1)
    cout << getMaxRepeatedSubstring("ababcabcab") << endl;          // (2, (3, 2))
    cout << getMaxRepeatedSubstring("abacdacdb") << endl;           // (2, (3, 2))
    cout << getMaxRepeatedSubstring("aaaaaaaa") << endl;            // (0, (1, 8))
    cout << getMaxRepeatedSubstring("xabcabcabcabcx") << endl;      // (1, (3, 4))
    cout << "-- kmp() ---------" << endl;
    cout << kmp("abcdefg", "cd") << endl;                           // (2)
    cout << kmp("ababcabcab", "ab") << endl;                        // (0, 2, 5, 8)
    cout << kmp("abacdacdb", "ac") << endl;                         // (2, 5)
    cout << kmp("aaaaaaaa", "a") << endl;                           // (0, 1, 2, 3, 4, 5, 6, 7)
    cout << kmp("aaaaaaaa", "aaa") << endl;                         // (0, 1, 2, 3, 4, 5)
    cout << kmp("xabcabcabcabcx", "abc") << endl;                   // (1, 4, 7, 10)
}
