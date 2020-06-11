#pragma once

struct UniqueValueCounter {
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
    //vector<int> values;

    PersistentSegmentTree treeAny;    // any times (>= 1)
    vector<int> rootAny;

    // v = compact values, uniqueValueN = the number unique values
    // 0 <= v[i] < uniqueValueN, O(NlogN)
    void buildWithCompactValues(const int values[], int n, int uniqueValueN) {
        this->N = n;
        //this->values.assign(values, values + n);

        vector<int> first(uniqueValueN, n), last(uniqueValueN, -1);
        vector<int> prev(n, -1), next(n, n);
        for (int i = 0; i < n; i++) {
            prev[i] = last[values[i]];
            last[values[i]] = i;
        }
        for (int i = n - 1; i >= 0; i--) {
            next[i] = first[values[i]];
            first[values[i]] = i;
        }

        //--- any-times values

        // build a persistent tree for range query
        /*       L            R
            x  x | x  x  x ... | ...
            0 +1
              -1  +1
        =>     0  +1
                  -1 +1
        =>     0   0 +1
        */
        rootAny.assign(n + 1, 0);
        rootAny[0] = treeAny.build(n);

        for (int i = 0; i < n; i++) {
            int r = rootAny[i];

            int p = prev[i];
            if (p >= 0)
                r = treeAny.add(r, p, -1);
            r = treeAny.add(r, i, 1);

            rootAny[i + 1] = r;
        }
    }

    void buildWithCompactValues(const vector<int>& values, int uniqueValueN) {
        buildWithCompactValues(values.data(), int(values.size()), uniqueValueN);
    }


    // return the number of values which appear in [left, right], inclusive
    // O(logN)
    int query(int left, int right) const {
        return treeAny.query(rootAny[right + 1], left, right);
    }
};
