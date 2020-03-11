#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "int128.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <climits>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testInt128() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Int128 -------------------------" << endl;
    {

        Int128 x(0ll, 0x7890123456789012ull);
        Int128 y(0ll, 0x1234567890123456ull);
        auto z = x + y;

        Int128 gt(0x0000000000000000ll, 0x8ac468ace68ac468ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) + (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0ll, 0x7890123456789012ull);
        Int128 y(0ll, 0x1234567890123456ull);
        auto z = x - y;

        Int128 gt(0x0000000000000000ll, 0x665bbbbbc6665bbcull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0ll, 0x7890123456789012ull);
        Int128 y(0ll, 0x1234567890123456ull);
        auto z = y - x;

        Int128 gt(0xffffffffffffffffll, 0x99a444443999a444ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   y.first, y.second, x.first, x.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        Int128 y(0ll, 0x1234567890123456ull);
        auto z = x + y;

        Int128 gt(0xffffffffffffff12ll, 0x8ac468ace68ac468ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) + (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        Int128 y(0ll, 0x1234567890123456ull);
        auto z = x - y;

        Int128 gt(0xffffffffffffff12ll, 0x665bbbbbc6665bbcull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        Int128 y(0ll, 0x1234567890123456ull);
        auto z = y - x;

        Int128 gt(0x00000000000000edll, 0x99a444443999a444ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   y.first, y.second, x.first, x.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        Int128 y(0xffffffffffffffffll, 0x1234567890123456ull);
        auto z = x + y;

        Int128 gt(0xffffffffffffff11ll, 0x8ac468ace68ac468ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) + (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        Int128 y(0xffffffffffffffffll, 0x1234567890123456ull);
        auto z = x - y;

        Int128 gt(0xffffffffffffff13ll, 0x665bbbbbc6665bbcull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        Int128 y(0xffffffffffffffffll, 0x1234567890123456ull);
        auto z = y - x;

        Int128 gt(0x00000000000000ecll, 0x99a444443999a444ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   y.first, y.second, x.first, x.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        Int128 x(0ll, 0x7890123456789012ull);
        auto y = 0x23423432423ll;
        auto z = x * Int128(y);

        Int128 gt(0x00000109ae036c85ll, 0x2b645e38f1743a76ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) * 0x%016llx = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        auto y = 0x23423432423ll;
        auto z = x * Int128(y);

        Int128 gt(0xfffdf490e597d3fbll, 0x2b645e38f1743a76ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) * 0x%016llx = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0xffffffffffffff12ll, 0x7890123456789012ull);
        Int128 y(0xffffffffffffffffll, 0xfffff23423432423ull);
        auto z = x * y;

        Int128 gt(0x000ccd07e4748e93ll, 0xa2633e38f1743a76ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) * (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        Int128 x(0x1234567890123456ll, 0x7890123456789012ull);
        auto y = 0x23423432423ll;
        auto z = x / Int128(y);

        Int128 gt(0x00000000000842d0ll, 0x6adaf50ae5e0a135ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) / 0x%016llx = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0x8234567890123456ll, 0x7890123456789012ull);
        Int128 y(0x123ll, 0x4567890123456789ull);
        auto z = x / y;

        Int128 gt(0xffffffffffffffffll, 0xff916fffffb5d7a0ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) / (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        Int128 x(0x1234567890123456ll, 0x7890123456789012ull);
        auto y = 0x23423432423ll;
        auto z = x % Int128(y);

        Int128 gt(0x0000000000000000ll, 0x000000cae23811d3ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) %% 0x%016llx = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        Int128 x(0x8234567890123456ll, 0x7890123456789012ull);
        Int128 y(0x123ll, 0x4567890123456789ull);
        auto z = x % y;

        Int128 gt(0xffffffffffffffffll, 0xce3ae2345646cb72ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) %% (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }

    //---

    {
        UInt128 x(0ull, 0x7890123456789012ull);
        UInt128 y(0ull, 0x1234567890123456ull);
        auto z = x + y;

        UInt128 gt(0x0000000000000000ull, 0x8ac468ace68ac468ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) + (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0ull, 0x7890123456789012ull);
        UInt128 y(0ull, 0x1234567890123456ull);
        auto z = x - y;

        UInt128 gt(0x0000000000000000ull, 0x665bbbbbc6665bbcull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0ull, 0x7890123456789012ull);
        UInt128 y(0ull, 0x1234567890123456ull);
        auto z = y - x;

        UInt128 gt(0xffffffffffffffffull, 0x99a444443999a444ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   y.first, y.second, x.first, x.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        UInt128 x(0xffffffffffffff12ull, 0x7890123456789012ull);
        UInt128 y(0ull, 0x1234567890123456ull);
        auto z = x + y;

        UInt128 gt(0xffffffffffffff12ull, 0x8ac468ace68ac468ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) + (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0xffffffffffffff12ull, 0x7890123456789012ull);
        UInt128 y(0ull, 0x1234567890123456ull);
        auto z = x - y;

        UInt128 gt(0xffffffffffffff12ull, 0x665bbbbbc6665bbcull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0xffffffffffffff12ull, 0x7890123456789012ull);
        UInt128 y(0ull, 0x1234567890123456ull);
        auto z = y - x;

        UInt128 gt(0x00000000000000edull, 0x99a444443999a444ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   y.first, y.second, x.first, x.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        UInt128 x(0xffffffffffffff12ull, 0x7890123456789012ull);
        UInt128 y(0xffffffffffffffffull, 0x1234567890123456ull);
        auto z = x + y;

        UInt128 gt(0xffffffffffffff11ull, 0x8ac468ace68ac468ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) + (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0xffffffffffffff12ull, 0x7890123456789012ull);
        UInt128 y(0xffffffffffffffffull, 0x1234567890123456ull);
        auto z = x - y;

        UInt128 gt(0xffffffffffffff13ull, 0x665bbbbbc6665bbcull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0xffffffffffffff12ull, 0x7890123456789012ull);
        UInt128 y(0xffffffffffffffffull, 0x1234567890123456ull);
        auto z = y - x;

        UInt128 gt(0x00000000000000ecull, 0x99a444443999a444ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) - (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   y.first, y.second, x.first, x.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        UInt128 x(0x12ll, 0x7890123456789012ull);
        auto y = 0x23423432423ll;
        auto z = x * UInt128(y);

        UInt128 gt(0x000028b428bbf6fbull, 0x2b645e38f1743a76ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) * 0x%016llx = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        UInt128 x(0x1234567890123456ull, 0x7890123456789012ull);
        auto y = 0x23423432423ull;
        auto z = x / UInt128(y);

        UInt128 gt(0x00000000000842d0ull, 0x6adaf50ae5e0a135ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) / 0x%016llx = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0x1234567890123456ull, 0x7890123456789012ull);
        UInt128 y(0x123ull, 0x4567890123456789ull);
        auto z = x / y;

        UInt128 gt(0x0000000000000000ull, 0x0010000000000000ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) / (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }

    {
        UInt128 x(0x1234567890123456ull, 0x7890123456789012ull);
        auto y = 0x23423432423ull;
        auto z = x % UInt128(y);

        UInt128 gt(0x0000000000000000ull, 0x000000cae23811d3ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) %% 0x%016llx = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y, z.first, z.second);
        }
        assert(z == gt);
    }
    {
        UInt128 x(0x1234567890123456ull, 0x7890123456789012ull);
        UInt128 y(0x123ull, 0x4567890123456789ull);
        auto z = x % y;

        UInt128 gt(0x0000000000000000ull, 0x0000123456789012ull);
        if (z != gt) {
            printf("Mismatched : (0x%016llx, 0x%016llx) %% (0x%016llx, 0x%016llx) = (0x%016llx, 0x%016llx)\n",
                   x.first, x.second, y.first, y.second, z.first, z.second);
        }
        assert(z == gt);
    }

    cout << "OK!" << endl;
}
