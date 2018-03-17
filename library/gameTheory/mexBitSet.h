#pragma once


#include "../set/bitSetVariable.h"

struct MexBitSet {
    BitSetVariable S;

    void insert(int x) {
        S.set(x);
    }

    int get() const {
        return S.firstClearBit();
    }
};
