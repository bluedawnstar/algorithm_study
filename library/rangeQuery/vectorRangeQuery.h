#pragma once

// Split & Build
template <typename T, typename MergeOp = function<T(T,T)>>
struct VectorRangeQuery {
    struct Block {
        Block* prev;
        Block* next;

        int idx;
        int cnt;
        vector<T> prefix;
        vector<T> suffix;

        void init(int K) {
            prefix.resize(K);
            suffix.resize(K);
            prev = nullptr;
            next = nullptr;
        }

        // O(1)
        void build(int i, T val) {
            idx = i;
            cnt = 1;
            prefix[0] = suffix[0] = val;
        }

        // O(sqrt(N))
        void build(int i, int n, const T* data, const MergeOp& mergeOp) {
            idx = i;
            cnt = n;

            prefix[0] = data[0];
            suffix[0] = data[n - 1];
            for (int i = 1, j = n - 2; i < n; i++, j--) {
                prefix[i] = mergeOp(prefix[i - 1], data[i]);
                suffix[i] = mergeOp(data[j], suffix[i - 1]);
            }
        }

        // O(sqrt(N))
        void eraseFirst(const T* data, const MergeOp& mergeOp) {
            idx++;
            cnt--;

            prefix[0] = data[0];
            for (int i = 1; i < cnt; i++)
                prefix[i] = mergeOp(prefix[i - 1], data[i]);
        }

        // O(sqrt(N))
        void eraseLast(const T* data, const MergeOp& mergeOp) {
            cnt--;

            suffix[0] = data[cnt - 1];
            for (int i = 1, j = cnt - 2; i < cnt; i++, j--)
                suffix[i] = mergeOp(data[j], suffix[i - 1]);
        }

        // O(sqrt(N))
        void updatePrefix(const T* data, const MergeOp& mergeOp) {
            prefix[0] = data[0];
            for (int i = 1; i < cnt; i++)
                prefix[i] = mergeOp(prefix[i - 1], data[i]);
        }

        // O(sqrt(N))
        void updatePrefix(int offset, const T* data, const MergeOp& mergeOp) {
            if (offset == 0) {
                prefix[0] = data[0];
                offset++;
            }
            for (int i = offset; i < cnt; i++)
                prefix[i] = mergeOp(prefix[i - 1], data[i]);
        }

        // O(sqrt(N))
        void updateSuffix(const T* data, const MergeOp& mergeOp) {
            suffix[0] = data[cnt - 1];
            for (int i = 1, j = cnt - 2; i < cnt; i++, j--)
                suffix[i] = mergeOp(data[j], suffix[i - 1]);
        }

        // O(sqrt(N))
        void updateSuffix(int offset, const T* data, const MergeOp& mergeOp) {
            if (offset == cnt - 1) {
                suffix[0] = data[cnt - 1];
                offset--;
            }
            for (int i = cnt - 1 - offset, j = offset; i < cnt; i++, j--)
                suffix[i] = mergeOp(data[j], suffix[i - 1]);
        }

        // O(1)
        T query() const {
            return prefix[cnt - 1];
        }

        // O(1)
        T queryPrefix(int R) const {
            return prefix[R];
        }

        // O(1)
        T querySuffix(int L) const {
            return suffix[cnt - 1 - L];
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

    T defaultValue;
    MergeOp mergeOp;

    VectorRangeQuery(const MergeOp& mop, T dfltValue, int maxN, int k = 0, int m = 0)
        : maxN(maxN), K(k), M(m), values(maxN), defaultValue(dfltValue), mergeOp(mop) {
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
            blocks[blockN++].build(i, min(K, n - i), v + i, mergeOp);

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
            blk->eraseFirst(&values[blk->idx + 1], mergeOp);
        } else if (idx == blk->cnt - 1) {
            blk->eraseLast(&values[blk->idx], mergeOp);
        } else {
            int i = blk->idx;
            int cnt = blk->cnt;

            blk = insertBlock(blk->next);
            blk->build(i + idx + 1, cnt - idx - 1, &values[i + idx + 1], mergeOp);
            blk->prev->cnt = idx;
            blk->prev->updateSuffix(&values[i], mergeOp);

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

        values[blk->idx + idx] = val;

        if (blk->cnt == 1)
            blk->prefix[0] = blk->suffix[0] = val;
        else {
            blk->updatePrefix(idx, &values[blk->idx], mergeOp);
            blk->updateSuffix(idx, &values[blk->idx], mergeOp);
        }
    }

    //--- query

    // O(sqrt(N))
    T query(int L, int R) {
        check();

        int idxL = L;
        Block* pL = findBlock(head, idxL);

        int idxR = R - L + idxL;
        Block* pR = findBlock(pL, idxR);

        T res = defaultValue;

        if (pL == pR) {
            res = mergeOp(res, queryRange(pL, idxL, idxR));
        } else {
            res = mergeOp(res, pL->querySuffix(idxL));
            for (pL = pL->next; pL != pR; pL = pL->next)
                res = mergeOp(res, pL->query());
            res = mergeOp(res, pR->queryPrefix(idxR));
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
            blocks[blockN++].build(i, min(K, valueN - i), &values[i], mergeOp);

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
            blk->build(i + idx, cnt - idx, &values[i + idx], mergeOp);
            blk->prev->cnt = idx;
            blk->prev->updateSuffix(&values[i], mergeOp);
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

    T queryRange(Block* blk, int L, int R) {
        T res = values[blk->idx + L++];
        while (L <= R)
            res = mergeOp(res, values[blk->idx + L++]);
        return res;
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
