#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(UnionFind);
    TEST(UnionFindWithCount);
    TEST(UnionFindSegment);
    TEST(BitSet);
    TEST(BitSetRangeUpdate);
    TEST(RangeSet);
    TEST(LineSegmentSet1D);
    TEST(SimpleHashMap);
    TEST(HashMapHAMT);
    TEST(SparseSet);
    TEST(BitSubsets);
    TEST(VanEmdeBoasTree);
}
