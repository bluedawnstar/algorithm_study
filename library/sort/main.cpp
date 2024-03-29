#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(SortSqrt);
    TEST(Sort);
    TEST(InversionCounter);
    TEST(InversionCounterAfterOneSwap);
    TEST(MinSwapsToSort);
    //--- problems
    TEST(SmallestBinaryArrayWithInversions);
}