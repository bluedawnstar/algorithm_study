#pragma once

//--------- Fenwick Tree (Binary Indexed Tree) --------------------------------

// for sum from 0 to pos
// 0 <= time (time 0 is initial state), 0 <= index < N
template <typename T>
struct PartiallyPersistentFenwickTree {
    vector<vector<int>> times;
    vector<vector<T>> tree;
    int currTime;

    explicit PartiallyPersistentFenwickTree(int n) : times(n + 1, vector<T>(1)), tree(n + 1, vector<T>(1)) {
        currTime = 1;
    }

    //--- query

    // sum from 0 to pos
    // O(logN)
    T sum(int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += getLastAt(pos);
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // O((logN)^2)
    T sum(int time, int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += getAt(time, pos);
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive, O(logN)
    T sumRange(int left, int right) const {
        T res = sum(right);
        if (left > 0)
            res -= sum(left - 1);
        return res;
    }

    // inclusive, O((logN)^2)
    T sumRange(int time, int left, int right) const {
        T res = sum(time, right);
        if (left > 0)
            res -= sum(time, left - 1);
        return res;
    }

    //--- update

    // return current time
    // O(logN)
    int add(int pos, T val) {
        addSub(pos, val);
        return currTime++;
    }

    // return current time
    // inclusive, O(logN)
    int addRange(int left, int right, T val) {
        addSub(left, val);
        if (right + 1 < int(tree.size()) - 1)
            addSub(right + 1, -val);
        return currTime++;
    }

    //--- point operations

    // O(logN)
    T get(int pos) const {
        T res = getLastAt(pos + 1);
        if (pos > 0) {
            int lca = pos & (pos + 1);
            for (pos; pos != lca; pos &= pos - 1)
                res -= getLastAt(pos);
        }

        return res;
    }

    // O((logN)^2)
    T get(int time, int pos) const {
        T res = getAt(time, pos + 1);
        if (pos > 0) {
            int lca = pos & (pos + 1);
            for (pos; pos != lca; pos &= pos - 1)
                res -= getAt(time, pos);
        }

        return res;
    }

    // return current time
    // O(logN)
    int set(int pos, T val) {
        addSub(pos, val - get(pos));
        return currTime++;
    }

//private:
    // O(logN)
    // 0 <= time (time 0 is initial state), 0 <= index < N
    T getAt(int time, int index) const {
        int i = int(upper_bound(times[index].begin(), times[index].end(), time) - times[index].begin());
        return tree[index][i - 1];
    }

    // O(1)
    T getLastAt(int index) const {
        return tree[index].back();
    }

    // O(1)
    // return current time
    void setAt(int index, T val) {
        times[index].push_back(currTime);
        tree[index].push_back(val);
    }

    // O(logN)
    void addSub(int pos, T val) {
        pos++;

        while (pos < int(tree.size())) {
            setAt(pos, getLastAt(pos) + val);
            pos += pos & -pos;      // add lowest bit
        }
    }
};


// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[0,i] >= sum)
// O(logN)
template <typename T>
int lowerBound(const PartiallyPersistentFenwickTree<T>& ft, T sum) {
    --sum;

    int N = int(ft.tree.size()) - 1;
    int pos = 0;

    int blockSize = N;
    while (blockSize & (blockSize - 1))
        blockSize &= blockSize - 1;

    for (; blockSize > 0; blockSize >>= 1) {
        int nextPos = pos + blockSize;
        if (nextPos < N && sum >= ft.getLastAt(nextPos)) {
            sum -= ft.getLastAt(nextPos);
            pos = nextPos;
        }
    }

    return pos;
}

// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[0,i] >= sum)
// O((logN)^2)
template <typename T>
int lowerBound(const PartiallyPersistentFenwickTree<T>& ft, int time, T sum) {
    --sum;

    int N = int(ft.tree.size()) - 1;
    int pos = 0;

    int blockSize = N;
    while (blockSize & (blockSize - 1))
        blockSize &= blockSize - 1;

    for (; blockSize > 0; blockSize >>= 1) {
        int nextPos = pos + blockSize;
        if (nextPos < N && sum >= ft.getAt(time, nextPos)) {
            sum -= ft.getAt(time, nextPos);
            pos = nextPos;
        }
    }

    return pos;
}


// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[0,i] >= sum)
// O((logN)^2)
template <typename T>
int findFirst(const PartiallyPersistentFenwickTree<T>& ft, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(left, mid) < sum)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return lo;
}

// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[i,N-1] < sum)
// O((logN)^2)
template <typename T>
int findLast(const PartiallyPersistentFenwickTree<T>& ft, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(mid, right) < sum)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}


// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[0,i] >= sum)
// O((logN)^3)
template <typename T>
int findFirst(const PartiallyPersistentFenwickTree<T>& ft, int time, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(time, left, mid) < sum)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return lo;
}

// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[i,N-1] < sum)
// O((logN)^3)
template <typename T>
int findLast(const PartiallyPersistentFenwickTree<T>& ft, int time, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(time, mid, right) < sum)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}
