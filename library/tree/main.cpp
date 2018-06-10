#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(TreeBasic);
    TEST(HeavyLightDecomposition);
    TEST(DfsTour);
    TEST(DfsTourTreePathSum);
    TEST(DfsTourTreeSubtreeSum);
    TEST(DfsTourTreeSubtreeUpdate);
    TEST(EulerTour);
    TEST(TreeMO);
    TEST(LcaSparseTable);
    TEST(LcaShieberVishkin);
    TEST(BST);
    TEST(RedBlackTree);
    TEST(RBTreeSimple);
    TEST(AVLTree);
    TEST(ScapegoatTree);
    TEST(Splay);
    TEST(SplayRangeQuery);
    TEST(Treap);
    TEST(TreapRangeQuery);
    TEST(LinkCutTree);
    TEST(LinkCutTreePathQuery);
    TEST(CentroidDecomposition);
    TEST(KDTree);
    TEST(KDTreeNoUpdate);
    TEST(IntervalTree);
    TEST(RTree);
    TEST(QuadTree);
    TEST(MetricTree);
    TEST(BlockTree);
    TEST(SparseTableOnTree);
    TEST(BlockTreeLCA);
    TEST(BlockTreePathQuery);
    TEST(DynamicBlockTreePathQuery);
    TEST(BlockTreeSubtreeUpdate);
    TEST(BlockTreeSubtreeUpdateDynamic);
    TEST(TreePathSqrtDecomposition);

    TEST(BenchmarkBST);
}