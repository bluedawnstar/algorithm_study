#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(FFT);
    TEST(Polynomial);
    TEST(PolynomialMod);
    TEST(Convolution);
    TEST(NTT);
    TEST(FactorialMod);
    TEST(RootFindingLaguerre);
    TEST(Lagrange);
}
