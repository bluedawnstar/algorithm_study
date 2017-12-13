#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(BitVectorRank);
    TEST(WaveletTree);
    TEST(WaveletTreeBitVector);
    TEST(WaveletMatrix);
    TEST(WaveletMatrixArray);
}