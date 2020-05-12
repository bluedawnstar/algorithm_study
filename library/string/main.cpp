#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(SuffixArray);
    TEST(SimpleSuffixArray);
    TEST(LcpArraySparseTable);
    TEST(SuffixSparseTable);
    TEST(SuffixAutomation);
    TEST(SuffixAutomationAM);
    TEST(SuffixTrie);
    TEST(SuffixTrieAM);
    TEST(SuffixTree);
    TEST(SuffixTreeAlgo);
    TEST(Trie);
    TEST(TrieAM);
    TEST(AhoCorasick);
    TEST(AhoCorasickAM);
    TEST(CompressedTrie);
    TEST(CompressedTrieAM);
    TEST(DistinctSubstringCounter);
    TEST(PalindromicSubsequence);
    TEST(PalindromicSubstring);
    TEST(PalindromicNumber);
    TEST(PalindromicTree);
    TEST(Split);
    TEST(LongestCommonSubstring_queryTwoStringAmongStrings);
    TEST(Hashing);
    TEST(SearchPrefixFunction);
    TEST(SearchRabinKarp);
    TEST(SearchZAlgorithm);
    TEST(PrefixFunction);
    TEST(Subsequence);
    TEST(SubsequenceLCS);
    TEST(LyndonFactorization);
    TEST(BurrowsWheelerTransform);
    TEST(SubstrHashing);
    TEST(LexicographicallyMinimalStringRotation);
    TEST(PalindromicFactorization);
    TEST(EerTree);
    TEST(SubsequenceDAG);
    TEST(DynamicStringHashWithTreap);
    TEST(DynamicStringHashWithSplayTree);
    TEST(RleString);
    TEST(PatternSubsequence);
    TEST(LongestDuplicatedSubstring);
    TEST(SuffixRollingHash);
    TEST(StringMatchingWithFFT);
    TEST(SimpleSubstring);
    TEST(ReverseSubstringsBetweenParentheses);
    TEST(BracketSequencesBalanced);
    TEST(SuffixArrayAlgoPrefixCounter);
}
