#include <functional>

using namespace std;

#include "calendar.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testCalendar() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Calendar ---------------------------" << endl;
    {
        assert(Calendar::getTotalDays(1, 1, 1) % 7 == 1);
        for (int i = 1; i <= 800; i++) {
            long long day1 = Calendar::getTotalDays(i, 1, 1);
            long long day2 = Calendar::getTotalDays(800 + i, 1, 1);
            if (day1 % 7 != day2 % 7)
                cout << "Mismatched : " << (day1 % 7) << ", " << (day2 % 7) << endl;
            assert(day1 % 7 == day2 % 7);
        }
    }

    cout << "OK!" << endl;
}
