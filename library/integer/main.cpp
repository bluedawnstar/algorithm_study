#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(PrimeNumberBasic);
    TEST(PrimeNumberEratosthenes);
    TEST(Gcd);
    TEST(IntMod);
    TEST(FactorialMod);
    TEST(Factor);
    TEST(PrimeFactor);
    TEST(EulerTheorem);
    TEST(Mobius);
    TEST(Digit);
    TEST(ChineseRemainderTheorem);
    TEST(DiophantineEquation);
    TEST(PrimitiveRoot);
    TEST(BigInt);
    TEST(Bit);
    TEST(PrimalityTest);
    TEST(PrimalityTestFast);
    TEST(PrimalityTestInt32);
    TEST(PrimalityTestInt64);
    TEST(PrimeFactorInt64);
    TEST(DiscreteEquations);
    TEST(GarnerAlgorithm);
    TEST(FastModOp);
    TEST(Int128);
    TEST(DiscreteLog);
}
