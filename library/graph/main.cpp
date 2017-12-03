#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(BasicDigraph);
    TEST(BasicUndirectedGraph);
    TEST(ShortestPath);
    TEST(BCC);
    TEST(DAG);
    TEST(BipartiteMaching);
    TEST(GeneralGraphMaxMatching);
    TEST(MinimumSpanningTree);
    TEST(Hungarian);
}
