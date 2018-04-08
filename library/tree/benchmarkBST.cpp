#include <cassert>
#include <functional>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

#include "binarySearchTree.h"
#include "redBlackTree.h"
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
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Benchmark Test of Binary Search Trees ---------------------" << endl;

    int N = 1000000;

    cout << "1) Insert sorted elements" << endl;
    {
        vector<int> in(N);
        iota(in.begin(), in.end(), 0);

        { cout << "set:       "; set<int> tree;                 testInsertSorted(tree, in); }
        { cout << "RBT:       "; RBTree<int> tree;              testInsertSorted(tree, in); }
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

        { cout << "set:       "; set<int> tree;                 testInsertAndSearchRandom(tree, in, key); }
        { cout << "RBT:       "; RBTree<int> tree;              testInsertAndSearchRandom(tree, in, key); }
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

        { cout << "set:       "; set<int> tree;                 testInsertAndEraseRandom(tree, in, key); }
        { cout << "RBT:       "; RBTree<int> tree;              testInsertAndEraseRandom(tree, in, key); }
        { cout << "AVL:       "; AVLTree<int> tree;             testInsertAndEraseRandom(tree, in, key); }
        { cout << "SGT(0.6):  "; ScapegoatTree<int> tree(0.6);  testInsertAndEraseRandom(tree, in, key); }
        { cout << "SGT(0.75): "; ScapegoatTree<int> tree(0.75); testInsertAndEraseRandom(tree, in, key); }
        { cout << "SGT(0.9):  "; ScapegoatTree<int> tree(0.9);  testInsertAndEraseRandom(tree, in, key); }
        { cout << "SPT:       "; SplayTree<int> tree;           testInsertAndEraseRandom(tree, in, key); }
        { cout << "TRP:       "; Treap<int> tree;               testInsertAndEraseRandom(tree, in, key); }
    }

    cout << "OK!" << endl;
}
