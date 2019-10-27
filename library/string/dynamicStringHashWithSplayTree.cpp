#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "dynamicStringHashWithSplayTree.h"

// https://www.hackerearth.com/challenges/competitive/february-circuits-19/algorithm/palindromes-3-7dbdca4b/

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static void dfsOut(ostream& os, DynamicStringHashWithSplayTree::Node* node) {
    if (!node)
        return;

    if (node->left)
        dfsOut(os, node->left);
    os << char(node->ch);
    if (node->right)
        dfsOut(os, node->right);
}

static ostream& operator <<(ostream& os, DynamicStringHashWithSplayTree& tr) {
    dfsOut(os, tr.tree);
    return os;
}

//---

void testDynamicStringHashWithSplayTree() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Dynamic String Hash with Splay Tree --------------------------" << endl;
    {
        RollingHashTable hash(100000);
        DynamicStringHashWithSplayTree tr(hash);

        tr.assign("aaa");
        cout << tr << endl;
        assert(tr.isPalindrom());

        tr.insert(1, 'b');           // "abaa"
        cout << tr << endl;
        assert(!tr.isPalindrom());

        tr.insert(3, 'b');           // "ababa"
        cout << tr << endl;
        assert(tr.isPalindrom());

        tr.insert(0, 'c');           // "cabaa"
        cout << tr << endl;
        assert(!tr.isPalindrom());

        tr.insert(6, 'c');           // "cababa"
        cout << tr << endl;
        assert(tr.isPalindrom());
    }
    cout << "OK!" << endl;
}
