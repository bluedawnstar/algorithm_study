#pragma once

template <typename T>
struct RangePointSet {
    map<T, T> lines;
    T covered = 0;                      // covered length

    RangePointSet() : covered(0) {
    }

    void clear() {
        lines.clear();
        covered = 0;
    }

    T getCovered() const {
        return covered;
    }


    vector<pair<T, T>> get() const {
        vector<pair<T, T>> res;
        res.reserve(lines.size());

        for (auto it = lines.begin(), itEnd = lines.end(); it != itEnd; ++it)
            res.push_back(*it);

        return res;
    }


    // inclusive [left, right], O(logN)
    bool exist(T left, T right) const {
        const auto it = get(left);
        return it != lines.end() && it->first <= right && right <= it->second;
    }

    // inclusive [left, right], O(logN)
    void add(T left, T right) {
        auto itL = lines.upper_bound(left);
        auto itR = lines.upper_bound(right + 1);

        if (itL != lines.begin()) {
            --itL;
            if (itL->second < left - 1)
                ++itL;
        }

        if (itL != itR) {
            left = min(left, itL->first);
            right = max(right, prev(itR)->second);
            for (auto it = itL; it != itR; ++it)
                covered -= it->second - it->first + 1;
            lines.erase(itL, itR);
        }

        lines[left] = right;
        covered += right - left + 1;
    }

    // inclusive [l, r], O(logN)
    void remove(T left, T right) {
        auto itL = lines.upper_bound(left);
        auto itR = lines.upper_bound(right);

        if (itL != lines.begin()) {
            --itL;
            if (itL->second < left)
                ++itL;
        }

        if (itL == itR)
            return;

        T tL = min(left, itL->first);
        T tR = max(right, prev(itR)->second);

        for (auto it = itL; it != itR; ++it)
            covered -= it->second - it->first + 1;
        lines.erase(itL, itR);

        if (tL < left) {
            lines[tL] = left - 1;
            covered += left - tL;
        }

        if (right < tR) {
            lines[right + 1] = tR;
            covered += tR - right;
        }
    }

private:
    // return the first range to include 'x' if exists, O(logN)
    typename map<T, T>::const_iterator get(T x) const {
        auto it = lines.upper_bound(x);
        if (it == lines.begin())
            return lines.end();

        --it;
        if (it->second < x)
            return lines.end();

        return it;
    }

    // return the first range to include 'x' if exists, O(logN)
    typename map<T, T>::iterator get(T x) {
        auto it = lines.upper_bound(x);
        if (it == lines.begin())
            return lines.end();

        --it;
        if (it->second < x)
            return lines.end();

        return it;
    }
};
