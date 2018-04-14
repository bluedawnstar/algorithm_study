#include <string>
#include <vector>

using namespace std;

#include "split.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <iostream>
#include "../common/iostreamhelper.h"

void testSplit() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- split() ---------" << endl;
    cout << split("a.b.c..", '.') << endl;
    cout << split(".b.c..", '.') << endl;
    cout << split(".b.c..d", '.') << endl;

    cout << "-- split2() ---------" << endl;
    cout << split2("a.b.c..", '.') << endl;
    cout << split2(".b.c..", '.') << endl;
    cout << split2(".b.c..d", '.') << endl;

    cout << "OK!" << endl;
}
