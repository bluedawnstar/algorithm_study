#include <functional>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

#include "deBruijnSequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testDeBruijnSequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- De Bruijn Sequence --------------------------------------" << endl;
    {
        assert(DeBruijnSequence::deBruijnAlphabet(2, 3, '0') == "00010111");
        assert(DeBruijnSequence::deBruijnAlphabet(4, 2, 'a') == "aabacadbbcbdccdd");
    }
    {
        auto ans1 = DeBruijnSequenceWithHamiltonianCircuit::deBruijnAlphabet(2, 3);
        assert(ans1 == "00010111");

        auto ans2 = DeBruijnSequenceWithHamiltonianCircuit::deBruijnAlphabet(4, 2, 'a');
        assert(ans2 == "aabacadbbcbdccdd");

        auto ans3 = DeBruijnSequenceWithIBWT::deBruijnAlphabet(2, 3);
        assert(ans3 == "00010111");

        auto ans4 = DeBruijnSequenceWithIBWT::deBruijnAlphabet(4, 2, 'a');
        assert(ans4 == "aabacadbbcbdccdd");
    }
    cout << "OK!" << endl;
}
