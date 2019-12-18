#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(MaxFlowEdmondsKarp);
    TEST(MaxFlowDinic);
    TEST(MaxFlowPushRelabel);
    TEST(MinCutMaxFlow);
    TEST(GomoryHuTree);
    TEST(MinCostMaxFlow);
    TEST(MinCostMaxFlowBellmanFord);
    TEST(MinCostMaxFlowPotential);
    TEST(MinCostMaxFlowSPFA);
    TEST(CirculationProblemWithVertexDemand);
    TEST(CirculationProblemWithEdgeDemand);
    TEST(MinCostCirculation);
    TEST(MaxCostCirculation);
}