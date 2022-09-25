#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(LargestRectangle);
    TEST(MinStack);
    TEST(MinQueue);
    TEST(MergeableHeap);
    TEST(QueueWith2Stacks);
    TEST(MaxHeap);
}
