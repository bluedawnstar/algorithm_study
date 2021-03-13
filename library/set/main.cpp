#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(UnionFind);
    TEST(UnionFindWithCount);
    TEST(UnionFindSegment);
    TEST(UnionFindWithValues);
    TEST(PersistentUnionFind);
    TEST(UndoableUnionFind);
    TEST(BitSet);
    TEST(BitSetRangeUpdate);
    TEST(DynamicRangeSet);
    TEST(LineSegmentSet1D);
    TEST(SimpleHashMap);
    TEST(HashMapHAMT);
    TEST(HashMapOAHT);
    TEST(SparseSet);
    TEST(BitSubsets);
    TEST(VanEmdeBoasTree);
}
