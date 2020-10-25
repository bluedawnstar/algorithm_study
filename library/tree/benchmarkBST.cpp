#include <cassert>
#include <functional>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

#include "binarySearchTree.h"
#include "redBlackTree.h"
#include "redBlackTreeSimple.h"
#include "redBlackTreeLeftLeaning.h"
#include "AVLTree.h"
#include "scapegoatTree.h"
#include "splayTree.h"
#include "treap.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <set>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"
#include "../common/profile.h"

/*
1) Insert sorted elements (N = 10^6)
    ------------+----------
      Type      | time
    ------------+----------
      set       | 0.149*
      RBT       | 0.237+
      SRBT      | 0.217+
      LLRBT     | 0.229+
      AVL       | 0.253+
      SGT(0.6)  | 1.154 !
      SGT(0.75) | 0.777
      SGT(0.9)  | 1.069 !
      SPT       | 0.046*
      TRP       | 0.167*
    ------------+----------

2) Insert random elements and Search elements (N = 10^6)
    ------------+---------+----------
      Type      | insert  | search
    ------------+---------+----------
      BST       | 0.810-  | 0.673+
      set       | 0.625*  | 0.681+
      RBT       | 0.749+  | 0.557*
      SRBT      | 0.756+  | 0.578*
      LLRBT     | 0.865-  | 0.608+
      AVL       | 0.933   | 0.594*
      SGT(0.6)  | 0.864-  | 0.634+
      SGT(0.75) | 0.811-  | 0.658+
      SGT(0.9)  | 0.849-  | 0.690+
      SPT       | 1.104 ! | 1.484 !
      TRP       | 1.085 ! | 0.846
    ------------+---------+----------

3) Insert random elements and Erase elements (N = 10^6)
    ------------+---------+----------
      Type      | insert  | erase
    ------------+---------+----------
      BST       | 0.813-  | 0.825+
      set       | 0.608*  | 0.914
      RBT       | 0.746+  | 0.780*
      SRBT      | 0.744+  | 0.759*
      LLRBT     | 0.892-  | 1.172 !
      AVL       | 0.906   | 0.879+
      SGT(0.6)  | 0.846-  | 0.965
      SGT(0.75) | 0.810-  | 0.924
      SGT(0.9)  | 0.847-  | 0.833+
      SPT       | 1.109 ! | 1.390 !
      TRP       | 1.092 ! | 1.097 !
    ------------+---------+----------
*/

template <typename TreeT>
static void testInsertSorted(TreeT& tr, const vector<int>& add) {
    PROFILE_START(0);
    for (auto x : add) {
        tr.insert(x);
    }
    PROFILE_STOP(0);

    if (tr.size() == 0)
        cout << "ERROR!" << endl;
}

static void testInsertAndSearchRandom(set<int>& tr, const vector<int>& add, const vector<int>& search) {
    PROFILE_START(0);
    for (auto x : add)
        tr.insert(x);
    PROFILE_STOP(0);

    if (tr.size() == 0)
        cout << "ERROR!" << endl;

    PROFILE_START(1);
    for (auto x : search) {
        auto it = tr.find(x);
        if (it == tr.end())
            cout << "ERROR!" << endl;
    }
    PROFILE_STOP(1);
}

template <typename TreeT>
static void testInsertAndSearchRandom(TreeT& tr, const vector<int>& add, const vector<int>& search) {
    PROFILE_START(0);
    for (auto x : add)
        tr.insert(x);
    PROFILE_STOP(0);

    if (tr.size() == 0)
        cout << "ERROR!" << endl;

    PROFILE_START(1);
    for (auto x : search) {
        auto it = tr.find(x);
        if (it == nullptr)
            cout << "ERROR!" << endl;
    }
    PROFILE_STOP(1);
}

template <typename TreeT>
static void testInsertAndEraseRandom(TreeT& tr, const vector<int>& add, const vector<int>& search) {
    PROFILE_START(0);
    for (auto x : add)
        tr.insert(x);
    PROFILE_STOP(0);

    if (tr.size() == 0)
        cout << "ERROR!" << endl;

    PROFILE_START(1);
    for (auto x : search)
        tr.erase(x);
    PROFILE_STOP(1);

    if (tr.size() != 0)
        cout << "ERROR!" << endl;
}

void testBenchmarkBST() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Benchmark Test of Binary Search Trees ---------------------" << endl;

    int N = 1000000;
#ifdef _DEBUG
    N = 100000;
#endif

    cout << "1) Insert sorted elements" << endl;
    {
        vector<int> in(N);
        iota(in.begin(), in.end(), 0);

        //{ cout << "BST:       "; BST<int> tree;                 testInsertSorted(tree, in); } // too much slow!!!
        { cout << "set:       "; set<int> tree;                 testInsertSorted(tree, in); }
        { cout << "RBT:       "; RBTree<int> tree;              testInsertSorted(tree, in); }
        { cout << "SRBT:      "; RBTreeSimple<int> tree;        testInsertSorted(tree, in); }
        { cout << "LLRBT:     "; LeftLeaningRBTree<int> tree;   testInsertSorted(tree, in); }
        { cout << "AVL:       "; AVLTree<int> tree;             testInsertSorted(tree, in); }
        { cout << "SGT(0.6):  "; ScapegoatTree<int> tree(0.6);  testInsertSorted(tree, in); }
        { cout << "SGT(0.75): "; ScapegoatTree<int> tree(0.75); testInsertSorted(tree, in); }
        { cout << "SGT(0.9):  "; ScapegoatTree<int> tree(0.9);  testInsertSorted(tree, in); }
        { cout << "SPT:       "; SplayTree<int> tree;           testInsertSorted(tree, in); }
        { cout << "TRP:       "; Treap<int> tree;               testInsertSorted(tree, in); }
    }
    cout << "2) Insert random elements and Search elements" << endl;
    {
        vector<int> in(N);
        vector<int> key(N);
        iota(in.begin(), in.end(), 0);
        iota(key.begin(), key.end(), 0);

        random_shuffle(in.begin(), in.end());
        random_shuffle(key.begin(), key.end());

        { cout << "BST:       "; BST<int> tree;                 testInsertAndSearchRandom(tree, in, key); }
        { cout << "set:       "; set<int> tree;                 testInsertAndSearchRandom(tree, in, key); }
        { cout << "RBT:       "; RBTree<int> tree;              testInsertAndSearchRandom(tree, in, key); }
        { cout << "SRBT:      "; RBTreeSimple<int> tree;        testInsertAndSearchRandom(tree, in, key); }
        { cout << "LLRBT:     "; LeftLeaningRBTree<int> tree;   testInsertAndSearchRandom(tree, in, key); }
        { cout << "AVL:       "; AVLTree<int> tree;             testInsertAndSearchRandom(tree, in, key); }
        { cout << "SGT(0.6):  "; ScapegoatTree<int> tree(0.6);  testInsertAndSearchRandom(tree, in, key); }
        { cout << "SGT(0.75): "; ScapegoatTree<int> tree(0.75); testInsertAndSearchRandom(tree, in, key); }
        { cout << "SGT(0.9):  "; ScapegoatTree<int> tree(0.9);  testInsertAndSearchRandom(tree, in, key); }
        { cout << "SPT:       "; SplayTree<int> tree;           testInsertAndSearchRandom(tree, in, key); }
        { cout << "TRP:       "; Treap<int> tree;               testInsertAndSearchRandom(tree, in, key); }
    }
    cout << "3) Insert random elements and Erase elements" << endl;
    {
        vector<int> in(N);
        vector<int> key(N);
        iota(in.begin(), in.end(), 0);
        iota(key.begin(), key.end(), 0);

        random_shuffle(in.begin(), in.end());
        random_shuffle(key.begin(), key.end());

        { cout << "BST:       "; BST<int> tree;                 testInsertAndEraseRandom(tree, in, key); }
        { cout << "set:       "; set<int> tree;                 testInsertAndEraseRandom(tree, in, key); }
        { cout << "RBT:       "; RBTree<int> tree;              testInsertAndEraseRandom(tree, in, key); }
        { cout << "SRBT:      "; RBTreeSimple<int> tree;        testInsertAndEraseRandom(tree, in, key); }
        { cout << "LLRBT:     "; LeftLeaningRBTree<int> tree;   testInsertAndEraseRandom(tree, in, key); }
        { cout << "AVL:       "; AVLTree<int> tree;             testInsertAndEraseRandom(tree, in, key); }
        { cout << "SGT(0.6):  "; ScapegoatTree<int> tree(0.6);  testInsertAndEraseRandom(tree, in, key); }
        { cout << "SGT(0.75): "; ScapegoatTree<int> tree(0.75); testInsertAndEraseRandom(tree, in, key); }
        { cout << "SGT(0.9):  "; ScapegoatTree<int> tree(0.9);  testInsertAndEraseRandom(tree, in, key); }
        { cout << "SPT:       "; SplayTree<int> tree;           testInsertAndEraseRandom(tree, in, key); }
        { cout << "TRP:       "; Treap<int> tree;               testInsertAndEraseRandom(tree, in, key); }
    }

    cout << "OK!" << endl;
}
