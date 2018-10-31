#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(SqrtDecomposition);
    TEST(SqrtDecompositionSum);
    TEST(MOAlgorithm);
    TEST(FenwickTree);
    TEST(FenwickTreeXor);
    TEST(FenwickTree2D);
    TEST(FenwickTreeMultAdd);
    TEST(FenwickTreeEx);
    TEST(GeneralizedBIT);
    TEST(PersistentFenwickTree);
    TEST(SegmentTree);
    TEST(SegmentTreePersistent);
    TEST(SegmentTreePartiallyPersistent);
    TEST(SegmentTreePersistentSimple);
    TEST(SegmentTreeCompact);
    TEST(SegmentTreeCompactLazy);
    TEST(SegmentTreeCompactLazyAdd);
    TEST(SegmentTreeMaxSubarray);
    TEST(SparseTable);
    TEST(SparseTable2D);
    TEST(DisjointSparseTable);
    TEST(SegmentTreeLine1D);
    TEST(SegmentTreeLine2D);
    TEST(SegmentTreeLine2DArray);
    TEST(SegmentTreeLineSegment2D);
    TEST(SegmentTreeLineSegment2DSum);
    TEST(SqrtTree);
    TEST(CompactSqrtTree);
    TEST(FastSqrtTree);
    TEST(MergeSortTree);
    TEST(MergeSortTreeIndex);
    TEST(ReverseSparseTable);
    TEST(SegmentTreeBitFlip);
    TEST(OfflineRMQ);
    TEST(SchieberVishkinRMQ);
    TEST(SegmentTree2DPoint);
}
