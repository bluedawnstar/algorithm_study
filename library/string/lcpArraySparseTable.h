#pragma once

#include "../tree/sparseTableMin.h"

// 1. making sequence
//    SA -> LCP array -> LcpArraySparseTable
// 2. This class is working based on SA indexes (not suffix position)
// 3. It can query LCP between arbitrary two suffix with SA indexes with O(1)
struct LcpArraySparseTable {
    SparseTableMin lcpTable;
    
    // PRECONDITION: lcpArray[i] = LCP(SA[i], SA[i - 1])
    template <typename T>
    LcpArraySparseTable(const T& lcpArray, int n)
        : lcpTable(lcpArray, n) {
        // no action
    }

    // inclusive (left SA index, right SA index)
    // CAUTION: if left == right, lcp() will return INT_MAX
    int lcp(int left, int right) {
        return lcpTable.query(left + 1, right);
    }
};
