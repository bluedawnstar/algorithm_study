#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(SuffixArray);
    TEST(LcpArraySparseTable);
    TEST(SuffixArraySparseTable);
    TEST(SuffixAutomation);
    TEST(SuffixAutomationAM);
    TEST(SuffixTrie);
    TEST(SuffixTrieAM);
    TEST(SuffixTree);
    TEST(SuffixTreeAlgo);
    TEST(Trie);
    TEST(TrieAM);
    TEST(AhoCorasickAM);
    TEST(CompressedTrie);
    TEST(CompressedTrieAM);
    TEST(DistinctSubstringCounter);
    TEST(PalindromicSubsequence);
    TEST(PalindromicString);
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
    TEST(SubstrHashing);
    TEST(LexicographicallyMinimalStringRotation);
    TEST(PalindromicFactorization);
    TEST(EerTree);
    TEST(SubsequenceDAG);
}
