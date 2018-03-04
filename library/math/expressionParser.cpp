#include <cmath>
#include <cctype>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

#include "expressionParser.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testShuntingyard() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Expression Parser -------------------" << endl;
    {
        auto gt = 1 + 2 * 3 * 4 + 3 * (2 + 2) - 100;
        auto ans = ExpressionParserShuntingYard::eval("1+2*3*4+3*(2+2)-100");

        cout << ans << endl;
        assert(gt == ans);
    }
    {
        auto gt = 1 + 2 * 3 * 4 + 3 * (2 + 2) - 100;
        ExpressionParserRecursiveDescent parser;
        auto ans = parser.eval("1+2*3*4+3*(2+2)-100");

        cout << ans << endl;
        assert(gt == ans);
    }

    cout << "OK!" << endl;
}
 