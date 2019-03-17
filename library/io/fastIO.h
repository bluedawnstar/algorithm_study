#pragma once

#include <stdio.h>

namespace FastIO {
    char gInBuff[1 << 24]; // sizeof in varied in problem
    const char* gInPtr;

    void init() {
        gInPtr = gInBuff;
        auto size = fread(gInBuff, 1, sizeof(gInBuff) - 4, stdin);
        gInBuff[size] = 0;
    }

    int readInt() {
        unsigned u = 0, s = 0;

        while (*gInPtr && *gInPtr <= 32)
            ++gInPtr;

        if (*gInPtr == '-') {
            s = ~s;
            ++gInPtr;
        } else if (*gInPtr == '+') {
            ++gInPtr;
        }

        while (*gInPtr >= '0' && *gInPtr <= '9')
            u = (u << 3) + (u << 1) + (*gInPtr++ - '0');

        return static_cast<int>((u ^ s) + !!s);
    }
}

/*
<Usage>

int main() {
    FastIO::init();

    int a = FastIO::readInt();
    int b = FastIO::readInt();
    // ...
}
*/
