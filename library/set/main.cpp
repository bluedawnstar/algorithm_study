#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(UnionFind);
    TEST(BitSet);
    TEST(BitSetRangeUpdate);
    TEST(RangeSet);
    TEST(SimpleHashMap);
    TEST(HashMapHAMT);
    TEST(SparseSet);
    TEST(BitSubsets);
    TEST(VanEmdeBoasTree);
}
