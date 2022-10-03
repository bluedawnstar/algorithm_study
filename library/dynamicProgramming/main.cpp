#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Subsequence);
    TEST(SubsequenceSpecial);
    TEST(LongestCommonSubsequence);
    TEST(PalindromicSubsequence);
    TEST(PalindromicSubstring);
    TEST(Knapsack);
    TEST(BitDP);
    TEST(DigitDP);
    TEST(DigitDP_IncreasingDigitSequence);
    TEST(DigitDP100Sum);
    TEST(DigitDPPalindromicNumber);
    TEST(ProductSumOfSubarray);
    TEST(PersistentConvexHull);
    TEST(DynamicUpperEnvelope);
    TEST(DynamicLowerEnvelope);
    TEST(DynamicConvexHull);
    TEST(ConvexHullTrick);
    TEST(DivideAndConquerOptimization);
    TEST(OptimalSquareDistance1D);
    TEST(KnuthOptimization);
    TEST(BitonicTour);
    TEST(EggDropping);
    TEST(BrokenProfile);
    TEST(BrokenProfileEx);
    TEST(PartitionProblem);
    TEST(PartitionProblemMultiple);
    TEST(BitLCS);
    TEST(CyclicLCS);
    TEST(LongestIncreasingSubsequence);
    TEST(LongestRepeatedSubsequence);
    TEST(MaxSubarraySum);
    TEST(SelectBlock2xN);
    TEST(IntegerPartition);
    TEST(TwoSameSumSubset);
    TEST(PowerSumOfAllCombinations);
    TEST(MaxNonoverlappingIntervals);
    TEST(MaxProfit);
    TEST(PalindromicPartitioning);
    TEST(BracketChangeCounter);
    TEST(SquareSubsets);
    TEST(GridPathCounterWithObstacles);
    TEST(GridMultistepPathCounterWithObstacles);
    TEST(MaxWeightOfAllConcatenatedStrings);
    TEST(GridConnectedComponentCounter);
    TEST(LongestZigzagSubsequence);
    TEST(PairCounterWithBitwiseOperation);
    TEST(PermutationCounterByDivisibleAdjacentPairCount);
    TEST(MaxSubsetAND);
    TEST(SubsequenceSum);
    TEST(LongestNonPeakSubsequence);

    TEST(SameDistanceTripleCounterOnTree);
    TEST(IndependentEdgeSetOnTree);
    TEST(TreePathDecompositionCounter);
    TEST(TopologicalOrderOnDAGLikeReversedTree);
    TEST(TraversalKnapsackOnTree);
    //--- problems
    TEST(ChefAndSum);
    TEST(DistinctValueSetCounterOfLinearCombination);
    TEST(IntervalSumTopK);
    TEST(DigitDP_SamePrefixSuffixDigitSum);
    TEST(KnapsackAndBinarySearchExample1);
    TEST(MaxSubsequenceLinearWeightedSum);
    TEST(SmallerStringCounter);
}
