#pragma once

#include "heapLazy.h"

// class for finding the median in a sliding window
template<typename T>
struct MedianFinder {
    HeapLazy<T, less<T>> lower;     // max Heap
    HeapLazy<T, greater<T>> upper;  // min Heap

    // amortized: O(logN)
    void push(T val) {
        if (!lower.empty() && val <= lower.top())
            lower.push(val);
        else
            upper.push(val);
        adjust();
    }

    // amortized: O(logN)
    void pop(T val) {
        if (!lower.empty() && val <= lower.top())
            lower.pop(val);
        else
            upper.pop(val);
        adjust();
    }

    // return sorted_value_array[N / 2]
    T median() {
        return upper.top();
    }

    // return (N/2, sum(sorted_value_array[0:N/2]))
    pair<int,long long> getLower() const {
        return make_pair(lower.size(), lower.totalSum());
    }

    // return ((N+1)/2, sum(sorted_value_array[N/2:N]))
    pair<int, long long> getUpper() const {
        return make_pair(upper.size(), upper.totalSum());
    }

private:
    void adjust() {
        int N = lower.size() + upper.size();
        while (lower.size() > N / 2) {
            upper.push(lower.top());
            lower.pop();
        }
        while (lower.size() < N / 2) {
            lower.push(upper.top());
            upper.pop();
        }
    }
};
