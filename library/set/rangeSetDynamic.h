#pragma once

template <typename T>
struct DynamicRangeSet : public map<T, T> {
    bool mergeAdjacentRange;

    explicit DynamicRangeSet(bool mergeAdjacentRange = true) : mergeAdjacentRange(mergeAdjacentRange) {
    }

    // return the first range to include 'x' if exists, O(logN)
    typename map<T,T>::const_iterator get(T x) const {
        auto it = this->upper_bound(x);
        if (it == this->begin())
            return this->end();

        --it;
        if (it->second < x)
            return this->end();

        return it;
    }

    // return the first range to include 'x' if exists, O(logN)
    typename map<T, T>::iterator get(T x) {
        auto it = this->upper_bound(x);
        if (it == this->begin())
            return this->end();

        --it;
        if (it->second < x)
            return this->end();

        return it;
    }

    vector<pair<T, T>> get() const {
        vector<pair<T, T>> res;
        res.reserve(this->size());

        for (auto it = this->begin(), itEnd = this->end(); it != itEnd; ++it)
            res.push_back(*it);

        return res;
    }


    // inclusive [left, right], O(logN)
    bool exist(T left, T right) const {
        const auto it = get(left);
        return it != this->end() && it->first <= right && right <= it->second;
    }

    // inclusive [left, right], O(logN)
    void insert(T left, T right) {
        auto itL = this->upper_bound(left);
        auto itR = this->upper_bound(right + mergeAdjacentRange);

        if (itL != this->begin()) {
            --itL;
            if (itL->second < left - mergeAdjacentRange)
                ++itL;
        }

        if (itL != itR) {
            left = min(left, itL->first);
            right = max(right, prev(itR)->second);
            this->erase(itL, itR);
        }

        (*this)[left] = right;
    }

    // inclusive [l, r], O(logN)
    void remove(T left, T right) {
        auto itL = this->upper_bound(left);
        auto itR = this->upper_bound(right);

        if (itL != this->begin()) {
            --itL;
            if (itL->second < left)
                ++itL;
        }

        if (itL == itR)
            return;

        T tL = min(left, itL->first);
        T tR = max(right, prev(itR)->second);

        this->erase(itL, itR);
        if (tL < left)
            (*this)[tL] = left - 1;

        if (right < tR)
            (*this)[right + 1] = tR;
    }
};
