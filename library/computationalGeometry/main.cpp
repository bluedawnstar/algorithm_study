#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Vector);
    TEST(Line);
    TEST(Polygon);
    TEST(PolygonInt);
    TEST(PolygonDiameter);
    TEST(ConvexHull);
    TEST(GridConvexHull);
    TEST(ConvexHullEx);
    TEST(DynamicConvexHull);
    TEST(LineSegmentUnion);
    TEST(LineSegmentSet1D);
    TEST(LineSegmentTree1D);
    TEST(LineSegmentIntersection);
    TEST(LineSegmentAllIntersections);
    TEST(DelaunayTriangulation);
    TEST(ClosestPair);
    TEST(DistanceSum);
    TEST(DynamicLowerEnvelope);
    TEST(DynamicUpperEnvelope);
    TEST(MaxHanhattanDistanceOfAllPairs);
    TEST(ConvexFunctions);
    TEST(Circle);
    //--- problems
    TEST(ConvexHullVisibleVerticalLine);
}
