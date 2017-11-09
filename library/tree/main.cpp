#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(TreeBasic);
    TEST(FenwickTree);
    TEST(FenwickTree2D);
    TEST(FenwickTreeMultAdd);
    TEST(SegmentTree);
    TEST(SparseTable);
    TEST(HeavyLightDecomposition);
    TEST(TreeMO);
    TEST(Trie);
    TEST(SuffixTrie);
    TEST(SuffixTree);
    TEST(SparseTableLCA);
    TEST(BST);
    TEST(Splay);
    TEST(SplayRangeQuery);
    TEST(Treap);
    TEST(TreapRangeQuery);
    TEST(LinkCutTree);
    TEST(LinkCutTreePathQuery);
    TEST(CentroidDecomposition);
    TEST(KDTree);
}