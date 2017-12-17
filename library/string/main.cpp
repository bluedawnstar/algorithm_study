#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(PrefixFunction);
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
    TEST(CompressedTrie);
    TEST(CompressedTrieAM);
    TEST(DistinctSubstringCounter);
    TEST(PalindromicSubsequence);
    TEST(PalindromicString);
    TEST(PalindromicTree);
    TEST(Split);
    TEST(LongestCommonSubstring_queryTwoStringAmongStrings);
    TEST(Hashing);
    TEST(ZAlgorithm);
}