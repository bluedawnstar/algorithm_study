#pragma once

// Split & Build

template <typename T>
struct SortedVector {
    struct Block {
        Block* prev;
        Block* next;

        int idx;
        int cnt;
        vector<T> v;

        void init(int K) {
            v.resize(K);
            prev = nullptr;
            next = nullptr;
        }

        void build(int i, T val) {
            idx = i;
            cnt = 1;
            v[0] = val;
        }

        void build(int i, int n, const T* data) {
            idx = i;
            cnt = n;

            memcpy(&v[0], data, sizeof(T) * n);
            sort(v.begin(), v.begin() + n);
        }

        void eraseFirst(T val) {
            int i = int(lower_bound(v.begin(), v.begin() + cnt, val) - v.begin());
            if (i < cnt - 1)
                memmove(&v[i], &v[i + 1], sizeof(T) * (cnt - i - 1));
            idx++;
            cnt--;
        }

        void eraseLast(T val) {
            int i = int(lower_bound(v.begin(), v.begin() + cnt, val) - v.begin());
            if (i < cnt - 1)
                memmove(&v[i], &v[i + 1], sizeof(T) * (cnt - i - 1));
            cnt--;
        }

        void update(T oldVal, T newVal) {
            int del = int(lower_bound(v.begin(), v.begin() + cnt, oldVal) - v.begin());
            int ins = int(lower_bound(v.begin(), v.begin() + cnt, newVal) - v.begin());
            
            if (ins > del + 1)
                memmove(&v[del], &v[del + 1], sizeof(T) * (--ins - del));
            else if (ins < del)
                memmove(&v[ins + 1], &v[ins], sizeof(T) * (del - ins));
            else
                ins = del;

            v[ins] = newVal;
        }

        int countLessOrEqual(T val) const {
            return int(upper_bound(v.begin(), v.begin() + cnt, val) - v.begin());
        }

        int countLess(T val) const {
            return int(lower_bound(v.begin(), v.begin() + cnt, val) - v.begin());
        }

        int countGreaterOrEqual(T val) const {
            return cnt - countLess(val);
        }

        int countGreater(T val) const {
            return cnt - countLessOrEqual(val);
        }

        int count(T val) const {
            return int(upper_bound(v.begin(), v.begin() + cnt, val) - lower_bound(v.begin(), v.begin() + cnt, val));
        }
    };

    int valueN;
    vector<T> values;

    int blockN;
    vector<Block> blocks;
    Block* unused;
    Block* head;
    Block* tail;

    int maxN;       // max count
    int K;          // max block size
    int M;          // max block count

    explicit SortedVector(int maxN, int k = 0, int m = 0) : maxN(maxN), K(k), M(m), values(maxN) {
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

    void build(const T* v, int n) {
        valueN = n;
        memcpy(&values[0], v, sizeof(T) * n);

        blockN = 0;
        for (int i = 0; i < n; i += K)
            blocks[blockN++].build(i, min(K, n - i), v + i);

        initLinkedList();
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }

    //--- update

    void insert(int pos, T x) {
        check();

        Block* blk = splitBlock(pos);
        blk = insertBlock(blk);
        values[valueN] = x;
        blk->build(valueN, values[valueN]);
        valueN++;
    }

    void erase(int pos) {
        if (pos >= valueN)
            return;

        check();

        int idx = pos;
        Block* blk = findBlock(head, idx);

        if (blk->cnt <= 1) {
            eraseBlock(blk);
            --blockN;
        } else if (idx == 0) {
            blk->eraseFirst(values[blk->idx]);
        } else if (idx == blk->cnt - 1) {
            blk->eraseLast(values[blk->idx + idx]);
        } else {
            int i = blk->idx;
            int cnt = blk->cnt;

            blk = insertBlock(blk->next);
            blk->build(i + idx + 1, cnt - idx - 1, &values[i + idx + 1]);
            blk->prev->build(i, idx, &values[i]);
        }
    }

    T get(int pos) {
        int idx = pos;
        Block* blk = findBlock(head, idx);
        return values[blk->idx + idx];
    }

    void update(int pos, T val) {
        int idx = pos;
        Block* blk = findBlock(head, idx);

        if (!blk)
            return;

        T oldVal = values[blk->idx + idx];
        values[blk->idx + idx] = val;

        if (blk->cnt == 1)
            blk->v[0] = val;
        else
            blk->update(oldVal, val);
    }

    //--- query

    int countLessOrEqual(int L, int R, T x) {
        check();

        int res = 0;
        for (Block *pL = splitBlock(L), *pR = splitBlock(pL, R - L + 1); pL != pR; pL = pL->next)
            res += pL->countLessOrEqual(x);

        return res;
    }

    int countLess(int L, int R, T x) {
        check();

        int res = 0;
        for (Block *pL = splitBlock(L), *pR = splitBlock(pL, R - L + 1); pL != pR; pL = pL->next)
            res += pL->countLess(x);

        return res;
    }

    int countGreaterOrEqual(int L, int R, T x) {
        check();

        int res = 0;
        for (Block *pL = splitBlock(L), *pR = splitBlock(pL, R - L + 1); pL != pR; pL = pL->next)
            res += pL->countGreaterOrEqual(x);

        return res;
    }

    int countGreater(int L, int R, T x) {
        check();

        int res = 0;
        for (Block *pL = splitBlock(L), *pR = splitBlock(pL, R - L + 1); pL != pR; pL = pL->next)
            res += pL->countGreater(x);

        return res;
    }

    int count(int L, int R, T x) {
        check();

        int res = 0;
        for (Block *pL = splitBlock(L), *pR = splitBlock(pL, R - L + 1); pL != pR; pL = pL->next)
            res += pL->count(x);

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
            blk->prev->build(i, idx, &values[i]);
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
