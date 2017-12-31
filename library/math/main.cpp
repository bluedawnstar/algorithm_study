#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Matrix);
    TEST(Fibonacci);
    TEST(TernarySearch);
    TEST(GaussianElimination);
    TEST(SimpsonIntegration);
    TEST(Shuntingyard);
}
