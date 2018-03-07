#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(SqrtDecomposition);
    TEST(MOAlgorithm);
    TEST(FenwickTree);
    TEST(FenwickTreeXor);
    TEST(FenwickTree2D);
    TEST(FenwickTreeMultAdd);
    TEST(FenwickTreeMax);
    TEST(SegmentTree);
    TEST(SegmentTreePersistent);
    TEST(SegmentTreeCompact);
    TEST(SegmentTreeCompactLazy);
    TEST(SegmentTreeCompactLazyAdd);
    TEST(SegmentTreeMaxSubarray);
    TEST(SparseTable);
    TEST(DisjointSparseTable);
    TEST(SegmentTreeLine1D);
    TEST(SegmentTreeLine2D);
    TEST(SegmentTreeLine2DArray);
    TEST(SegmentTreeLineSegment2D);
    TEST(SegmentTreeLineSegment2DSum);
}
