// uncomment one of below macros

//#define TEST_TREE_BASIC
#define TEST_TREE_MO

#if defined(TEST_TREE_BASIC)
#include "treeBasic.cpp"
int main(void) {
    testTreeBasic();
}
#elif defined(TEST_TREE_MO)
#include "treeMO.cpp"
int main(void) {
    testTreeMO();
}
#endif
