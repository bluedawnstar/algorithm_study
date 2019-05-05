#pragma once

// The number of values which appear exactly K times in a range
template <typename T>
struct KTimesValueCounter {
    struct PersistentSegmentTree {
        int N;
        vector<int> value;
        vector<int> L;
        vector<int> R;

        PersistentSegmentTree() {
        }

        // return root node index
        int build(int n) {
            N = n;
            value.reserve(N * 4);
            L.reserve(N * 4);
            R.reserve(N * 4);

            return dfsBuild(0, N - 1);
        }

        // return root node index, O(logN)
        int add(int root, int index, int val) {
            return dfsAdd(root, 0, N - 1, index, val);
        }

        // O(logN)
        int query(int root, int left, int right) const {
            return dfsQuery(root, 0, N - 1, left, right);
        }

    private:
        int dfsBuild(int left, int right) {
            int l = -1, r = -1;
            if (left < right) {
                int mid = (left + right) >> 1;
                l = dfsBuild(left, mid);
                r = dfsBuild(mid + 1, right);
            }
            value.push_back(0);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsAdd(int node, int left, int right, int index, int val) {
            int newValue;
            int l = -1, r = -1;
            if (left == right) {
                newValue = value[node] + val;
            } else {
                int mid = (left + right) >> 1;
                if (index <= mid) {
                    l = dfsAdd(L[node], left, mid, index, val);
                    r = R[node];
                } else {
                    l = L[node];
                    r = dfsAdd(R[node], mid + 1, right, index, val);
                }
                newValue = value[l] + value[r];
            }
            value.push_back(newValue);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsQuery(int node, int left, int right, int indexL, int indexR) const {
            if (indexR < left || right < indexL)
                return 0;

            if (indexL <= left && right <= indexR)
                return value[node];

            int mid = (left + right) >> 1;
            return dfsQuery(L[node], left, mid, indexL, indexR) + dfsQuery(R[node], mid + 1, right, indexL, indexR);
        }
    };

    int N;
    vector<T> values;
    vector<T> uniqueValues;

    int K;                  // occurence count
    PersistentSegmentTree tree;
    vector<int> roots;

    // k = occurrence count
    // O(NlogN)
    void build(const T v[], int n, int k) {
        this->N = n;
        this->K = k;

        // value compaction
        values.assign(v, v + n);
        uniqueValues.assign(v, v + n);
        sort(uniqueValues.begin(), uniqueValues.end());
        uniqueValues.erase(unique(uniqueValues.begin(), uniqueValues.end()), uniqueValues.end());
        for (int i = 0; i < n; i++)
            values[i] = int(lower_bound(uniqueValues.begin(), uniqueValues.end(), values[i]) - uniqueValues.begin());

        // prepare
        int valueN = int(uniqueValues.size());

        vector<int> first(valueN, n), last(valueN, -1);
        vector<int> prev(n, -1), next(n, n);
        for (int i = 0; i < n; i++) {
            prev[i] = last[values[i]];
            last[values[i]] = i;
        }
        for (int i = n - 1; i >= 0; i--) {
            next[i] = first[values[i]];
            first[values[i]] = i;
        }

        vector<int> rangeFirst(n, -1);  // first position of a range with K values
        if (k == 1) {
            iota(rangeFirst.begin(), rangeFirst.end(), 0);
        } else {
            for (int i = 0; i < valueN; i++) {
                int front = first[i];
                int rear = front;
                for (int j = 1; j < k && rear < n; j++)
                    rear = next[rear];
                while (rear < n) {
                    rangeFirst[rear] = front;
                    front = next[front];
                    rear = next[rear];
                }
            }
        }

        // build a persistent tree for range query
        /*       L           R
        x  0 | 1 2 3 ... K | K+1 ...
        -1 +1
        +1 -2  +1 
        =>  0 -1  +1
        */
        roots.assign(n + 1, 0);
        roots[0] = tree.build(n);

        for (int i = 0; i < n; i++) {
            int r = roots[i];
            if (rangeFirst[i] >= 0) {
                int pp = -1;
                int p = -1;

                p = prev[rangeFirst[i]];
                if (p >= 0)
                    pp = prev[p];

                if (pp >= 0)
                    r = tree.add(r, pp, 1);
                if (p >= 0)
                    r = tree.add(r, p, -2);
                r = tree.add(r, rangeFirst[i], 1);
            }
            roots[i + 1] = r;
        }
    }

    void build(const vector<T>& v, int k) {
        build(v.data(), int(v.size()), k);
    }


    // return the number of values which appear exactly K times in [left, right], inclusive
    // O(logN)
    int query(int left, int right) const {
        if (right - left + 1 < K)
            return 0;
        return tree.query(roots[right + 1], left, right);
    }
};
