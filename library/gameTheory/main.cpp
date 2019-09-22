#include <iostream>

using namespace std;

#define TEST(ModuleName)    extern void test##ModuleName(void); \
                            test##ModuleName()

int main(void) {
    TEST(MexTrie);
    TEST(WythoffGame);
    TEST(PirateGame);
    TEST(MinimumKnightMoves);
}
