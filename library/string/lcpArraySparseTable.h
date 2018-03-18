#pragma once

#include "../rangeQuery/sparseTableMin.h"

// 1. making sequence
//    SA -> LCP array -> LcpArraySparseTable
// 2. This class is working based on SA indexes (not suffix position)
// 3. It can query LCP between arbitrary two suffix with SA indexes with O(1)
struct LcpArraySparseTable {
    SparseTableMin lcpTable;
    
    LcpArraySparseTable() {
    }


    // PRECONDITION: lcpArray[i] = LCP(SA[i], SA[i - 1])
    LcpArraySparseTable(const int lcpArray[], int n)
        : lcpTable(lcpArray, n) {
    }

    LcpArraySparseTable(const vector<int>& lcpArray)
        : lcpTable(lcpArray) {
    }


    // PRECONDITION: lcpArray[i] = LCP(SA[i], SA[i - 1])
    void build(const int lcpArray[], int n) {
        lcpTable.build(lcpArray, n);
    }

    void build(const vector<int>& lcpArray) {
        lcpTable.build(lcpArray);
    }


    // inclusive (left SA index, right SA index)
    // CAUTION: if left == right, lcp() will return INT_MAX
    int lcp(int left, int right) const {
        return lcpTable.query(left + 1, right);
    }
};
