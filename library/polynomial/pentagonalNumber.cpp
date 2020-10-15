#include <vector>
#include <algorithm>

using namespace std;

#include "pentagonalNumber.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testPentagonalNumber() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Pentagonal Number theorem ----------------" << endl;
    {
        /*
0, 1, 2, 5, 7, 12, 15, 22, 26, 35, 40, 51, 57, 70, 77, 92, 100, 117, 126, 145, 155, 176, 187, 210, 222, 247, 260, 287, 301, 330, 345, 376, 392, 425, 442, 477, 495, 532, 551, 590, 610, 651, 672, 715, 737, 782, 805, 852, 876, 925, 950
        */
        vector<pair<int, int>> gt{
            {  0,  1}, {  1, -1}, {  2, -1}, {  5,  1}, {  7,  1}, { 12, -1}, { 15, -1}, { 22,  1}, { 26,  1},
            { 35, -1}, { 40, -1}, { 51,  1}, { 57,  1}, { 70, -1}, { 77, -1}, { 92,  1}, {100,  1}, {117, -1},
            {126, -1}, {145,  1}, {155,  1}, {176, -1}, {187, -1}, {210,  1}, {222,  1}, {247, -1}, {260, -1},
            {287,  1}, {301,  1}, {330, -1}, {345, -1}, {376,  1}, {392,  1}, {425, -1}, {442, -1}, {477,  1},
            {495,  1}, {532, -1}, {551, -1}, {590,  1}, {610,  1}, {651, -1}, {672, -1}, {715,  1}, {737,  1},
            {782, -1}, {805, -1}, {852,  1}, {876,  1}, {925, -1}, {950, -1}
        };
        auto sequence = PentagonalNumber::generalizedPentagonalNumbers(1000);
        if (sequence != gt) {
            cout << "Mismatched : " << sequence << ", " << gt << endl;
        }
        assert(sequence == gt);
    }

    cout << "OK!" << endl;
}
