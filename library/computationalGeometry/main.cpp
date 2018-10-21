#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Vector);
    TEST(Line);
    TEST(Polygon);
    TEST(PolygonDiameter);
    TEST(ConvexHull);
    TEST(DynamicConvexHull);
    TEST(LineSegmentUnion);
    TEST(LineSegmentSet1D);
    TEST(LineSegmentTree1D);
    TEST(LineSegmentIntersection);
    TEST(LineSegmentAllIntersections);
    TEST(DelaunayTriangulation);
    TEST(ClosestPair);
    TEST(DistanceSum);
}
