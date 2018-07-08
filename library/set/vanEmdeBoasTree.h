#pragma once

// All keys belong to the range [0, u - 1]

//template <typename T = long long>
template <typename T = int>
struct VanEmdeBoasTree {
    static const T NaN = T(-1);

    T u;                                        // 
    T sqrtU;                                    // 

    T minX;                                     // 
    T maxX;                                     //
    unique_ptr<VanEmdeBoasTree> summary;        // 
    vector<unique_ptr<VanEmdeBoasTree>> cluster;// 

    explicit VanEmdeBoasTree(T _u) {
        assert(_u >= 2);

        u = _u;
        sqrtU = T(ceil(sqrt(u)));

        minX = NaN;
        maxX = NaN;

        if (u > 2) {
            summary.reset(new VanEmdeBoasTree(sqrtU));
            cluster.resize(sqrtU);
        }
    }

    bool empty() const {
        return minX == NaN;
    }

    T min() const {
        return minX;
    }

    T max() const {
        return maxX;
    }

    // O(log log u)
    bool exist(T x) const {
        if (x < 0 || x >= u)
            return false;

        if (u == 2) {
            if (x == 0)
                return minX == 0;
            else if (x == 1)
                return maxX == 1;
            return false;
        } else {
            if (minX == NaN)
                return false;
            if (x < minX || x > maxX)
                return false;
            else if (x == minX || x == maxX)
                return true;
            else {
                T hi = x / sqrtU;
                T lo = x % sqrtU;
                if (cluster[hi] == nullptr)
                    return false;
                else
                    return cluster[hi]->exist(lo);
            }
        }
    }

    // O(log log u)
    void insert(T x) {
        if (x < 0 || x >= u)
            return;
        if (x == minX || x == maxX)
            return;

        if (u == 2) {
            if (minX == NaN)
                minX = maxX = x;
            else if (x == 0)
                minX = x;
            else if (x == 1)
                maxX = x;
        } else {
            if (minX == NaN) {
                minX = maxX = x;
            } else {
                if (x < minX)
                    swap(x, minX);

                T hi = x / sqrtU;
                T lo = x % sqrtU;
                if (cluster[hi] == nullptr) {
                    cluster[hi].reset(new VanEmdeBoasTree(sqrtU));
                    cluster[hi]->insert(lo);
                    summary->insert(hi);
                } else {
                    cluster[hi]->insert(lo);
                }
                if (x > maxX)
                    maxX = x;
            }
        }
    }

    // O(log log u)
    void erase(T x) {
        if (minX == NaN || x < minX || x > maxX)
            return;

        if (u == 2) {
            if (x == 0) {
                if (maxX == 0)
                    minX = maxX = NaN;
                else
                    minX = 1;
            } else if (x == 1) {
                if (minX == 1)
                    minX = maxX = NaN;
                else
                    maxX = 0;
            }
        } else {
            if (x == minX) {
                if (x == maxX) {
                    minX = maxX = NaN;
                } else {
                    T nextMinHi = summary->min();
                    T nextMinLo = cluster[nextMinHi]->min();
                    T nextMin = nextMinHi * sqrtU + nextMinLo;

                    cluster[nextMinHi]->erase(nextMinLo);
                    if (cluster[nextMinHi]->minX == NaN) {
                        summary->erase(nextMinHi);
                        cluster[nextMinHi].reset();
                    }
                    minX = nextMin;
                }
            } else {
                T hi = x / sqrtU;
                T lo = x % sqrtU;

                if (cluster[hi] != nullptr) {
                    cluster[hi]->erase(lo);
                    if (cluster[hi]->minX == NaN) {
                        summary->erase(hi);
                        cluster[hi].reset();
                    }
                }
                if (x == maxX) {
                    if (summary->minX == NaN)
                        maxX = minX;
                    else {
                        T nextMaxHi = summary->max();
                        T nextMaxLo = cluster[nextMaxHi]->max();
                        maxX = nextMaxHi * sqrtU + nextMaxLo;
                    }
                }
            }
        }
    }

    // O(log log u)
    T prev(T x) const {
        if (minX == NaN || x <= minX)
            return NaN;
        else if (x > maxX)
            return maxX;

        if (u == 2) {
            return 0;
        } else {
            T hi = x / sqrtU;
            T lo = x % sqrtU;

            if (cluster[hi] == nullptr) {
                T prv = summary->prev(hi);
                if (prv == NaN)
                    return minX;
                return prv * sqrtU + cluster[prv]->max();
            } else {
                T newLo = NaN;
                T newHi = hi;
                T minInCluster = cluster[hi]->min();
                if (lo > minInCluster)
                    newLo = cluster[hi]->prev(lo);
                else {
                    newHi = summary->prev(hi);
                    if (newHi != NaN)
                        newLo = cluster[newHi]->max();
                }
                if (newLo == NaN || newHi == NaN)
                    return minX;
                return newHi * sqrtU + newLo;
            }
        }
    }

    // O(log log u)
    T next(T x) const {
        if (minX == NaN || x >= maxX)
            return NaN;
        else if (x < minX)
            return minX;

        if (u == 2) {
            return 1;
        } else {
            T hi = x / sqrtU;
            T lo = x % sqrtU;

            if (cluster[hi] == nullptr) {
                T nxt = summary->next(hi);
                if (nxt == NaN)
                    return NaN;
                return nxt * sqrtU + cluster[nxt]->min();
            } else {
                T newLo = NaN;
                T newHi = hi;
                T maxInCluster = cluster[hi]->max();
                if (lo < maxInCluster)
                    newLo = cluster[hi]->next(lo);
                else {
                    newHi = summary->next(hi);
                    if (newHi != NaN)
                        newLo = cluster[newHi]->min();
                }
                if (newLo == NaN || newHi == NaN)
                    return NaN;
                return newHi * sqrtU + newLo;
            }
        }
    }

    // O(log log u)
    T popMin() {
        T res = min();
        if (res != NaN)
            erase(res);
        return res;
    }

    // O(log log u)
    T popMax() {
        T res = max();
        if (res != NaN)
            erase(res);
        return res;
    }
};
