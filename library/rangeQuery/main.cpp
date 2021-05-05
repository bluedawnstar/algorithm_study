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
    TEST(PersistentFenwickTreeMultAdd);
    TEST(SegmentTree);
    TEST(SegmentTreePersistent);
    TEST(SegmentTreePartiallyPersistent);
    TEST(SegmentTreePersistentSimple);
    TEST(SegmentTreePersistentLazySimple);
    TEST(SegmentTreeCompact);
    TEST(SegmentTreeCompactLazy);
    TEST(SegmentTreeCompactLazyAdd);
    TEST(SegmentTreeCompactLazyEx);
    TEST(SegmentTreeMaxSubarray);
    TEST(DynamicSegmentTree);
    TEST(DynamicSegmentTreeMaxSubarray);
    TEST(DynamicSegmentTreeForest);
    TEST(SparseTable);
    TEST(SparseTable2D);
    TEST(DisjointSparseTable);
    TEST(SparseTableSimpleRMQ);
    TEST(SegmentTreeLine1D);
    TEST(SegmentTreeLine2D);
    TEST(SegmentTreeLine2DArray);
    TEST(SegmentTreeLineSegment2D);
    TEST(SegmentTreeLineSegment2DSum);
    TEST(SqrtTree);
    TEST(CompactSqrtTree);
    TEST(FastSqrtTree);
    TEST(MergeSortTree);
    TEST(MergeSortTreeWithSum);
    TEST(MergeSortTreeIndex);
    TEST(MergeSortTree2DPoint);
    TEST(MergeSortTree2DPoint_Dynamic);
    TEST(MergeSortTree2DPoint_DynamicFast);
    TEST(MergeSortTreeCompact);
    TEST(MergeSortTreeCompactWithSum);
    TEST(MergeSortTreeConvexHull);
    TEST(ReverseSparseTable);
    TEST(SegmentTreeBitFlip);
    TEST(OfflineRMQ);
    TEST(SchieberVishkinRMQ);
    TEST(KeyRangeCommandDecomposerSimple);
    TEST(RedBlackTreeRangeSum);
    TEST(BinarySearchTreeRangeSum);
    TEST(PersistentBinarySearchTreeRangeSum);
    TEST(IntTrie);
    TEST(IntTriePersistent);
    TEST(KTimesValueCounter);
    TEST(UniqueValueCounter);
    TEST(VectorRangeCount);
    TEST(VectorRangeSum);
    TEST(VectorRangeQuery);
    TEST(GcdOnSubsegments);
    TEST(MaxConsecutiveSubarray);
    TEST(DivideCombineTree);
    TEST(SubsetXorSegmentTree);
    TEST(SegmentTreeBeats);
    TEST(SegmentTreePrimeFactorXorRollingHash);
    TEST(LongestIncreasingSubsequence);
    TEST(LongestIncreasingStep);
    TEST(BitTrieForest);
}
