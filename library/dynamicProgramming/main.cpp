#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(ConvexHullTrick);
    TEST(KnuthOptimization);
    TEST(DevideAndConquerOptimization);
    TEST(Subsequence);
    TEST(PalindromicSubsequence);
    TEST(PalindromicSubstring);
    TEST(Knapsack);
    TEST(BitDP);
    TEST(DigitDP);
    TEST(ProductSumOfSubarray);
    TEST(PersistentConvexHull);
}
