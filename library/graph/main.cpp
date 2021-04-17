#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(BasicDigraph);
    TEST(BasicUndirectedGraph);
    TEST(ShortestPath);
    TEST(ShortestPathAllPairs);
    TEST(ReachableAllPairs);
    TEST(BCC);
    TEST(BlockCutTree);
    TEST(DAG);
    TEST(MinPathCoverDAG);
    TEST(BipartiteMaching);
    TEST(BipartiteGraphCheck);
    TEST(BipartiteGraphMaxMatchingInMatrix);
    TEST(BipartiteGraphTransformMatrixWithFlip);
    TEST(GeneralMaxMatching);
    TEST(MinimumSpanningTree);
    TEST(Hungarian);
    TEST(GraphColoringGreedy);
    TEST(SAT2);
    TEST(MaxClique);
    TEST(MaxIndependentSet);
    TEST(Hamiltonian);
    TEST(GlobalMinCut);
    TEST(EulerianDigraph);
    TEST(DominatorTree);
    TEST(GraphRealization);
    TEST(PathCount);
    TEST(BridgeBlockTree);
    TEST(ChordalGraph);
    TEST(ChordalGraphToCliqueTree);
    TEST(ShortestPathWithWildcard);
    TEST(SpecialMST);
    TEST(GlobalMinCutUnweightedKarger);
    TEST(BipartiteWeightedMaching);
    TEST(GeneralMaxMatchingWeighted);
    TEST(DirectedMST);
    TEST(RangeBasedDenseGraph);
    TEST(ShortestPathGraph);
}
