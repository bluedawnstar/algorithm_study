#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Matrix);
    TEST(Fibonacci);
    TEST(TernarySearch);
    TEST(GaussianElimination);
    TEST(BooleanXorExpression);
    TEST(SimpsonIntegration);
    TEST(Shuntingyard);
    TEST(FactorialZeroes);
    TEST(WeightedMedian);
    TEST(Fraction);
    TEST(FareySequence);
    TEST(SternBrocotSequence);
    TEST(SternBrocotFractionFinder);
    TEST(SternBrocotTree);
    TEST(LinearRecurrence);
    TEST(SumOfPower);
    TEST(LongestArithmeticSubsequence);
    TEST(RoundRobinTournament);
}
