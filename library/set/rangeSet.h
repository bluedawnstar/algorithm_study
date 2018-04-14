#pragma once

template <typename T>
struct RangeSet : public map<T, T> {
    bool mergeAdjacentRange;

    explicit RangeSet(bool mergeAdjacentRange = true) : mergeAdjacentRange(mergeAdjacentRange) {
    }

    // return the first range to include 'left' if exists
    // O(logN)
    typename map<T,T>::const_iterator get(T left) const {
        auto it = upper_bound(left);
        if (it == begin() || (--it)->second < left)
            return end();
        return it;
    }

    // return the first range to include 'left' if exists
    // O(logN)
    typename map<T, T>::iterator get(T left) {
        auto it = upper_bound(left);
        if (it == begin() || (--it)->second < left)
            return end();
        return it;
    }

    vector<pair<T, T>> get() const {
        vector<pair<T, T>> res;
        res.reserve(size());

        for (auto it = begin(), itEnd = end(); it != itEnd; ++it)
            res.push_back(*it);

        return res;
    }


    // inclusive [left, right], O(logN)
    bool exist(T left, T right) const {
        const auto it = get(left);
        return it != end() && it->first <= right && right <= it->second;
    }

    // inclusive [left, right], O(logN)
    void insert(T left, T right) {
        auto itL = upper_bound(left);
        auto itR = upper_bound(right + mergeAdjacentRange);

        if (itL != begin()) {
            if ((--itL)->second < left - mergeAdjacentRange)
                ++itL;
        }

        if (itL != itR) {
            left = min(left, itL->first);
            right = max(right, prev(itR)->second);
            erase(itL, itR);
        }

        (*this)[left] = right;
    }

    // inclusive [l, r], O(logN)
    void remove(T left, T right) {
        auto itL = upper_bound(left);
        auto itR = upper_bound(right);

        if (itL != begin()) {
            if ((--itL)->second < left)
                ++itL;
        }

        if (itL == itR)
            return;

        T tL = min(left, itL->first);
        T tR = max(right, prev(itR)->second);

        erase(itL, itR);
        if (tL < left)
            (*this)[tL] = left - 1;

        if (right < tR)
            (*this)[right + 1] = tR;
    }
};
