#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(MergeableHeap);
    TEST(MaxHeap);
    TEST(HeapLazy);
    TEST(MedianFinder);
}
