#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Comb);
    TEST(CombModExt);
    TEST(NextCombination);
    TEST(NextPermutation);
    TEST(DeBruijnSequence);
    TEST(StirlingNumber);
    TEST(PruferCode);
    TEST(KirchhoffTheorem);
    TEST(Catalan);
    TEST(LatinSquare);
    TEST(MaxMatchingInMatrix);
    TEST(IntegerPartition);
    TEST(MinManhattanDistanceSet);
}
