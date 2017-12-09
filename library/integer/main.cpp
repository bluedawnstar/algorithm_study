#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(PrimeNumberBasic);
    TEST(PrimeNumberEratosthenes);
    TEST(Gcd);
    TEST(IntMod);
    TEST(Factor);
    TEST(PrimeFactor);
    TEST(Fraction);
    TEST(Digit);
    TEST(ChineseRemainderTheorem);
    TEST(Equations);
}
