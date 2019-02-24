#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "rleString.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static ostream& operator <<(ostream& os, RleString& rs) {
    for (auto it : rs.S) {
        for (int i = 0; i < it.second; i++)
            cout << it.first;
    }
    return os;
}

//---

void testRleString() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- RLE String --------------------------" << endl;
    {
        RleString rs;

        rs.assign("aaa");
        cout << rs << endl;
        assert(rs.isPalindrom());

        rs.insert(1, 'b');           // "abaa"
        cout << rs << endl;
        assert(!tr.isPalindrom());

        rs.insert(3, 'b');           // "ababa"
        cout << rs << endl;
        assert(rs.isPalindrom());

        rs.insert(0, 'c');           // "cabaa"
        cout << rs << endl;
        assert(!rs.isPalindrom());

        rs.insert(6, 'c');           // "cababa"
        cout << rs << endl;
        assert(rs.isPalindrom());
    }
    cout << "OK!" << endl;
}
