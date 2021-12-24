#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Matrix);
    TEST(Fibonacci);
    TEST(TernarySearch);
    TEST(TernarySearch3D);
    TEST(GaussianElimination);
    TEST(BooleanXorExpression);
    TEST(SimpsonIntegration);
    TEST(Shuntingyard);
    TEST(FactorialZeroes);
    TEST(WeightedMedian);
    TEST(Fraction);
    TEST(LatticePoints);
    TEST(FareySequence);
    TEST(SternBrocotSequence);
    TEST(SternBrocotFractionFinder);
    TEST(SternBrocotTree);
    TEST(LinearRecurrence);
    TEST(SumOfPower);
    TEST(LongestArithmeticSubsequence);
    TEST(RoundRobinTournament);
    TEST(JosephusProblem);
    TEST(GeneratedDistinctNumbersCounterByBitwiseOperation);
    TEST(GolombRuler);
    TEST(ArrayGeneratorWithDistinctSubarraySums);
    TEST(TransformOperationByAddOrMult);
    TEST(ProuhetTarryEscottProblem);
    TEST(InversionCounter);
    TEST(MatrixPowerSum);
    TEST(Simplex);
    TEST(DifferenceConstraintsSystem);
    TEST(DifferenceConstraintsSystem2);
    TEST(PythagoreanTriples);
    //--- problems
    TEST(SimplexPolygonChain);
    TEST(TransformMatrix);
}
