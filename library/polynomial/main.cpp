#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(FFT);
    TEST(FFT2);
    TEST(PolyFFT);
    TEST(PolyFFTMod);
    TEST(Convolution);
    TEST(NTT);
    TEST(FactorialMod);
    TEST(RootFindingLaguerre);
    TEST(Lagrange);
    TEST(PowerSumPolyMod);
    TEST(WalshHadamard);
    TEST(VandermondeMatrix);
    TEST(Polynomial);
    TEST(FactorialIntModFast);
    TEST(PowerSeries);
    TEST(PentagonalNumber);
    TEST(PolynomialSum);
    TEST(ModComplex);
}
