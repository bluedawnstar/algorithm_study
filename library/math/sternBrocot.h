#pragma once

#include "fractionAlgo.h"

//TODO: implement functions about Stern Brocot Tree

//--------- Stern-Brocot Tree -------------------------------------------------

// inclusive range
template <typename T, typename U = T>
pair<T, T> findKthSternBrocot(T denomRangeMin, T denomRangeMax, pair<T, T> left, pair<T, T> right, T rateL, T rateR, U cnt) {
    pair<T, T> mid = { rateL * left.first + rateR * right.first, rateL * left.second + rateR * right.second };
    auto count = sumFloorRange(denomRangeMin, denomRangeMax, mid.first, mid.second);
    T cntEq = denomRangeMax / mid.second - (denomRangeMin - 1) / mid.second;

    if (count - cntEq < cnt && cnt <= count)
        return mid;

    if (count >= cnt) {
        return (rateR == 1) ? findKthSternBrocot(denomRangeMin, denomRangeMax, left, mid, 2 * rateL, T(1), cnt)
            : findKthSternBrocot(denomRangeMin, denomRangeMax, left, right, T(1), rateR / 2, cnt);
    } else {
        return (rateL == 1) ? findKthSternBrocot(denomRangeMin, denomRangeMax, mid, right, T(1), 2 * rateR, cnt)
            : findKthSternBrocot(denomRangeMin, denomRangeMax, left, right, rateL / 2, T(1), cnt);
    }
}

// inclusive range
template <typename T, typename U = T>
pair<T, T> findKthSternBrocot(T denomRangeMin, T denomRangeMax, U cnt) {
    return findKthSternBrocot(denomRangeMin, denomRangeMax, make_pair<T, T>(0, 1), make_pair<T, T>(1, 0), T(1), T(1), cnt);
}
