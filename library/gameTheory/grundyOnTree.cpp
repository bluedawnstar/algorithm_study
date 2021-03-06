#include <functional>
#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#include "mexTrie.h"
#include "mexTriePersistent.h"

#include "grundyOnTree.h"
#include "grundyOnTree2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testGrundyOnTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Grundy On a Tree -----------------------" << endl;
    //--- test GrundyOnTree
    {
        GrundyOnTree<8> gtree(5);
        gtree.addEdge(0, 1);
        gtree.addEdge(1, 2);
        gtree.addEdge(0, 3);
        gtree.addEdge(3, 4);
        gtree.build(0);

        assert(gtree.getGrundy(0) == 0);
    }
    {
        GrundyOnTree<8> gtree(5);
        gtree.addEdge(0, 1);
        gtree.addEdge(0, 2);
        gtree.addEdge(0, 3);
        gtree.addEdge(0, 4);
        gtree.build(0);

        assert(gtree.getGrundy(0) != 0);
    }
    {
        GrundyOnTree<8> gtree(6);
        gtree.addEdge(0, 1);
        gtree.addEdge(1, 2);
        gtree.addEdge(1, 3);
        gtree.addEdge(3, 4);
        gtree.addEdge(1, 5);
        gtree.build(0);

        assert(gtree.getGrundy(0) != 0);
    }
    //--- test GrundyOnTree2
    {
        GrundyOnTree2<8> gtree(5);
        gtree.addEdge(0, 1);
        gtree.addEdge(1, 2);
        gtree.addEdge(0, 3);
        gtree.addEdge(3, 4);
        gtree.build(0);

        assert(gtree.getGrundy(0) == 0);
    }
    {
        GrundyOnTree2<8> gtree(5);
        gtree.addEdge(0, 1);
        gtree.addEdge(0, 2);
        gtree.addEdge(0, 3);
        gtree.addEdge(0, 4);
        gtree.build(0);

        assert(gtree.getGrundy(0) != 0);
    }
    {
        GrundyOnTree2<8> gtree(6);
        gtree.addEdge(0, 1);
        gtree.addEdge(1, 2);
        gtree.addEdge(1, 3);
        gtree.addEdge(3, 4);
        gtree.addEdge(1, 5);
        gtree.build(0);

        assert(gtree.getGrundy(0) != 0);
    }

    cout << "OK!" << endl;
}
