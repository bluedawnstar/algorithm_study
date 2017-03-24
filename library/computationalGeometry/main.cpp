#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(Vector);
    TEST(Line);
    TEST(Polygon);
    TEST(ConvexHull);
}
