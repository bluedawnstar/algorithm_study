#pragma once

#include <stdio.h>

namespace FastIO {
    const int SIZE = (1 << 24);

    char gInBuff[SIZE];
    const char* gInPtr;

    void init() {
        gInPtr = gInBuff;
        auto sz = fread(gInBuff, 1, SIZE - 1, stdin);
        gInBuff[sz] = 0;
    }

    // with sign
    int readSInt() {
        unsigned int u = 0, s = 0;

        while (*gInPtr && *gInPtr <= 32)
            ++gInPtr;

        if (*gInPtr == '-') {
            s = ~s;
            ++gInPtr;
        } else if (*gInPtr == '+') {
            ++gInPtr;
        }

        while (*gInPtr >= '0' && *gInPtr <= '9')
            u = u * 10 + (*gInPtr++ - '0');

        return static_cast<int>((u ^ s) + !!s);
    }

    int readInt() {
        int res = 0;

        while (*gInPtr && *gInPtr <= 32)
            ++gInPtr;

        while (*gInPtr >= '0' && *gInPtr <= '9')
            res = res * 10 + (*gInPtr++ - '0');

        return res;
    }

    // with sign
    long long readSLL() {
        unsigned long long u = 0, s = 0;

        while (*gInPtr && *gInPtr <= 32)
            ++gInPtr;

        if (*gInPtr == '-') {
            s = ~s;
            ++gInPtr;
        } else if (*gInPtr == '+') {
            ++gInPtr;
        }

        while (*gInPtr >= '0' && *gInPtr <= '9')
            u = u * 10 + (*gInPtr++ - '0');

        return static_cast<long long>((u ^ s) + !!s);
    }

    long long readLL() {
        long long res = 0;

        while (*gInPtr && *gInPtr <= 32)
            ++gInPtr;

        while (*gInPtr >= '0' && *gInPtr <= '9')
            res = res * 10 + (*gInPtr++ - '0');

        return res;
    }

    struct Stream {
        Stream() {
            init();
        }

        Stream& operator >>(int& x) {
            x = readInt();
            return *this;
        }

        Stream& operator >>(long long& x) {
            x = readLL();
            return *this;
        }
    };
}

/*
<Usage>

int main() {
    FastIO::init();

    int a = FastIO::readInt();
    int b = FastIO::readInt();
    // ...
}

OR

int main() {
    FastIO::Stream is;

    int a, b;
    is >> a >> b;
    // ...
}

*/
