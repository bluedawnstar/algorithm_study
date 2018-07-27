#pragma once

// Split & Build
template <typename T>
struct VectorRangeSum {
    struct Block {
        Block* prev;
        Block* next;

        int idx;
        int cnt;
        vector<T> v;

        void init(int K) {
            v.resize(K + 1);
            prev = nullptr;
            next = nullptr;
        }

        // O(1)
        void build(int i, T val) {
            idx = i;
            cnt = 1;
            v[0] = 0;
            v[1] = val;
        }

        // O(sqrt(N))
        void build(int i, int n, const T* data) {
            idx = i;
            cnt = n;

            v[0] = 0;
            for (int i = 1; i <= n; i++)
                v[i] = v[i - 1] + data[i - 1];
        }

        // O(1)
        void eraseLast() {
            cnt--;
        }

        // O(sqrt(N))
        void update(int offset, T delta) {
            for (int i = offset + 1; i <= cnt; i++)
                v[i] += delta;
        }

        // O(1)
        T sum() const {
            return v[cnt] - v[0];
        }

        // O(1)
        T sumRange(int L, int R) const {
            return v[R + 1] - v[L];
        }
    };

    int maxN;       // max count
    int K;          // max block size
    int M;          // max block count

    int valueN;
    vector<T> values;

    int blockN;
    vector<Block> blocks;
    Block* unused;
    Block* head;
    Block* tail;

    explicit VectorRangeSum(int maxN, int k = 0, int m = 0) : maxN(maxN), K(k), M(m), values(maxN) {
        if (K <= 0)
            K = int(ceil(sqrt(maxN)));
        if (M <= 0)
            M = ((maxN + K - 1) / K) * 4;

        unused = nullptr;
        head = nullptr;
        tail = nullptr;

        blocks.resize(M);
        for (int i = 0; i < M; i++)
            blocks[i].init(K);

        valueN = 0;
        blockN = 0;
    }

    //--- build


    // O(N * log(N))
    void build(const T* v, int n) {
        valueN = n;
        memcpy(&values[0], v, sizeof(T) * n);

        blockN = 0;
        for (int i = 0; i < n; i += K)
            blocks[blockN++].build(i, min(K, n - i), v + i);

        initLinkedList();
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }

    //--- update

    // O(sqrt(N))
    void insert(int pos, T x) {
        check();

        Block* blk = splitBlock(pos);
        blk = insertBlock(blk);
        values[valueN] = x;
        blk->build(valueN, values[valueN]);
        valueN++;
    }

    // O(sqrt(N))
    void erase(int pos) {
        if (pos >= valueN)
            return;

        int idx = pos;
        Block* blk = findBlock(head, idx);

        if (blk->cnt <= 1) {
            eraseBlock(blk);
            --blockN;
        } else if (idx == 0) {
            blk->build(blk->idx + 1, blk->cnt - 1, &values[blk->idx + 1]);
        } else if (idx == blk->cnt - 1) {
            blk->eraseLast();
        } else {
            int i = blk->idx;
            int cnt = blk->cnt;

            blk = insertBlock(blk->next);
            blk->build(i + idx + 1, cnt - idx - 1, &values[i + idx + 1]);
            blk->prev->cnt = idx;

            check();
        }
    }

    // O(sqrt(N))
    T get(int pos) {
        int idx = pos;
        Block* blk = findBlock(head, idx);
        return values[blk->idx + idx];
    }

    // O(sqrt(N))
    void update(int pos, T val) {
        int idx = pos;
        Block* blk = findBlock(head, idx);

        if (!blk)
            return;

        T delta = val - values[blk->idx + idx];
        values[blk->idx + idx] = val;

        if (blk->cnt == 1) {
            blk->v[0] = 0;
            blk->v[1] = val;
        } else
            blk->update(idx, delta);
    }

    //--- query

    // O(sqrt(N))
    T sum(int L, int R) {
        check();

        T res = 0;

        int idxL = L;
        Block* pL = findBlock(head, idxL);

        int idxR = R - L + idxL;
        Block* pR = findBlock(pL, idxR);

        if (pL == pR) {
            res = sumRange(pL, idxL, idxR);
        } else {
            res = sumSuffix(pL, idxL);
            for (pL = pL->next; pL != pR; pL = pL->next)
                res += pL->sum();
            res += sumPrefix(pR, idxR);
        }

        return res;
    }

private:
    void rebuild() {
        vector<int> v(values.size());

        valueN = 0;
        for (Block* p = head; p; p = p->next) {
            memcpy(&v[valueN], &values[p->idx], sizeof(T) * p->cnt);
            valueN += p->cnt;
        }

        swap(values, v);

        blockN = 0;
        for (int i = 0; i < valueN; i += K)
            blocks[blockN++].build(i, min(K, valueN - i), &values[i]);

        initLinkedList();
    }

    void check() {
        if (blockN >= M - 2)
            rebuild();
    }

    //--- block management

    Block* findBlock(Block* start, int& idx) {
        Block* blk = start;
        while (blk && idx >= blk->cnt) {
            idx -= blk->cnt;
            blk = blk->next;
        }
        return blk;
    }

    // return the pointer of next block
    Block* splitBlock(Block* start, int idx) {
        Block* blk = findBlock(start, idx);
        if (idx) {
            int i = blk->idx;
            int cnt = blk->cnt;

            blk = insertBlock(blk->next);
            blk->build(i + idx, cnt - idx, &values[i + idx]);
            blk->prev->cnt = idx;
        }
        return blk;
    }

    Block* splitBlock(int idx) {
        return splitBlock(head, idx);
    }

    Block* insertBlock(Block* blk) {
        Block* newBlock = popUnusedBlock();
        insertBlockBefore(blk, newBlock);
        ++blockN;
        return newBlock;
    }

    //--- naive operations

    T sumPrefix(Block* blk, int R) {
        return blk->v[R + 1] - blk->v[0];
    }

    T sumSuffix(Block* blk, int L) {
        return blk->v[blk->cnt] - blk->v[L];
    }

    T sumRange(Block* blk, int L, int R) {
        return blk->v[R + 1] - blk->v[L];
    }

    //--- linked list

    void initLinkedList() {
        if (blockN == 0)
            head = tail = nullptr;
        else {
            for (int i = 1; i < blockN; i++) {
                blocks[i - 1].next = &blocks[i];
                blocks[i].prev = &blocks[i - 1];
            }
            head = &blocks[0];
            tail = &blocks[blockN - 1];
            head->prev = nullptr;
            tail->next = nullptr;
        }

        if (blockN >= M)
            unused = nullptr;
        else {
            for (int i = blockN + 1; i < M; i++)
                blocks[i - 1].next = &blocks[i];
            blocks[M - 1].next = nullptr;
            unused = &blocks[blockN];
        }
    }

    void pushUnusedBlock(Block* blk) {
        blk->next = unused;
        unused = blk;
    }

    Block* popUnusedBlock() {
        Block* res = unused;
        unused = unused->next;
        return res;
    }

    void insertBlockBefore(Block* pos, Block* newBlock) {
        if (!head) {
            newBlock->prev = nullptr;
            newBlock->next = nullptr;
            head = newBlock;
            tail = newBlock;
        } else if (pos == nullptr) {
            tail->next = newBlock;
            newBlock->prev = tail;
            newBlock->next = nullptr;
            tail = newBlock;
        } else {
            if (pos->prev)
                pos->prev->next = newBlock;
            else
                head = newBlock;
            newBlock->prev = pos->prev;
            newBlock->next = pos;
            pos->prev = newBlock;
        }
    }

    void eraseBlock(Block* pos) {
        if (pos == head)
            head = head->next;
        if (pos == tail)
            tail = tail->prev;
        if (pos->prev)
            pos->prev->next = pos->next;
        if (pos->next)
            pos->next->prev = pos->prev;
        pushUnusedBlock(pos);
    }
};
