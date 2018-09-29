#pragma once

template <typename T, typename Compare = less<T>>
struct SchieberVishkinRMQ {
    Compare compare;

    vector<unsigned int> indices;
    vector<unsigned int> inlabel;
    vector<unsigned int> ascendant;
    vector<unsigned int> head;

    SchieberVishkinRMQ() {
    }

    SchieberVishkinRMQ(const T value[], int N) {
        build(value, N);
    }

    explicit SchieberVishkinRMQ(const vector<T>& value) {
        build(value);
    }


    void build(const T value[], int N) {
        vector<int> left(N, -1);
        vector<int> right(N, -1);
        vector<int> stk(N);

        // build Cartesian Tree
        for (int i = 0, top = 0; i < N; ++i) {
            int last = -1;
            while (top > 0 && compare(value[i], value[stk[top - 1]]))
                last = stk[--top];

            if (top > 0)
                right[stk[top - 1]] = i;

            left[i] = last;
            stk[top++] = i;
        }

        // find preorder
        int root = stk[0];
        vector<int> parents(N, -1);
        vector<int> order;

        indices.resize(N);
        inlabel.resize(N);
        for (int top = 1; top > 0; ) {
            int u = stk[--top];
            order.push_back(u);
            indices[u] = inlabel[u] = order.size();

            if (left[u] != -1) {
                stk[top++] = left[u];
                parents[left[u]] = u;
            }

            if (right[u] != -1) {
                stk[top++] = right[u];
                parents[right[u]] = u;
            }
        }

        // calc helper structures for Schieber-Vishkin LCA
        ascendant.resize(N);
        head.resize(N);
        for (int i = N - 1; i > 0; --i) {
            int v = order[i];
            int p = parents[v];
            if (lsb(inlabel[p]) < lsb(inlabel[v]))
                inlabel[p] = inlabel[v];
        }

        ascendant[root] = 0;
        for (int i = 1; i < N; ++i) {
            int v = order[i];
            int p = parents[v];
            ascendant[v] = ascendant[p] | lsb(inlabel[v]);
        }

        head[0] = root;
        for (int i = 1; i < N; ++i) {
            int v = order[i];
            int p = parents[v];
            if (inlabel[v] != inlabel[p])
                head[indices[v] - 1] = p;
            else
                head[indices[v] - 1] = head[indices[p] - 1];
        }
    }

    void build(const vector<T>& value) {
        build(value.data(), int(value.size()));
    }


    // return the index of the minimum value in [left, right] in O(1)
    int queryIndex(int left, int right) const {
        unsigned int Iv = inlabel[right];
        unsigned int Iu = inlabel[left];
        unsigned int hIv = lsb(Iv);
        unsigned int hIu = lsb(Iu);
        unsigned int mask = msb((Iv ^ Iu) | hIv | hIu) - 1;
        unsigned int j = lsb(ascendant[right] & ascendant[left] & ~mask);
        int x, y;

        if (j == hIv)
            x = right;
        else {
            mask = msb(ascendant[right] & (j - 1)) - 1;
            x = head[((indices[right] & ~mask) | (mask + 1)) - 1];
        }

        if (j == hIu)
            y = left;
        else {
            mask = msb(ascendant[left] & (j - 1)) - 1;
            y = head[((indices[left] & ~mask) | (mask + 1)) - 1];
        }

        return indices[x] < indices[y] ? x : y;
    }

private:
    static unsigned int lsb(unsigned int x) {
        return x & (~x + 1);
    }

    static unsigned int msb(unsigned int x) {
        if (x == 0)
            return 1u;

#ifndef __GNUC__
        return 1u << (31 - __lzcnt(x));
#else
        return 1u << (31 - __builtin_clz(x));
#endif
    }
};
