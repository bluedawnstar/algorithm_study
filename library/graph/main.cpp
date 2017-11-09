#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(ShortestPath);
    TEST(MinCostMaxFlow);
    TEST(BasicDigraph);
    TEST(BCC);
    TEST(TopologicalSort);
}
