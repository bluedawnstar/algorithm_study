#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(PrefixFunction);
    TEST(SuffixArray);
    TEST(SuffixTree);
    TEST(LcpArraySparseTable);
    TEST(SuffixArraySparseTable);
    TEST(DistinctSubstringCounter);
    TEST(Split);
}