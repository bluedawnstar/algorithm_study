#pragma once

// - command[i] = { key min, key max, value }
// - query[i] = key
//   -> process all commands to include the key
struct SimpleKeyRangeQueryDecomposer {
    int minKey;
    int maxKey;
    int keyBlockSize;
    int keyBlockCount;
    vector<vector<tuple<int, int, int>>> cmdBlock;

    // 1000000000 / 2500000 = 400
    explicit SimpleKeyRangeQueryDecomposer(
        int minKey = 0, int maxKey = 1000000000 - 1, int keyBlockSize = 2500000)
        : minKey(minKey), maxKey(maxKey),
          keyBlockSize(keyBlockSize), keyBlockCount((maxKey + keyBlockSize - 1) / keyBlockSize),
          cmdBlock(keyBlockCount) {
    }

    void add(int keyMin, int keyMax, int value) {
        int l = max(0, keyMin) / keyBlockSize;
        int r = min(maxKey, keyMax) / keyBlockSize;
        while (l <= r)
            cmdBlock[l++].emplace_back(keyMin, keyMax, value);
    }

    vector<tuple<int, int, int>>& getCmdList(int key) {
        int idx = key / keyBlockSize;
        return cmdBlock[idx];
    }

    void query(int key, const function<void(int)>& op) {
        int idx = key / keyBlockSize;

        auto& v = cmdBlock[idx];
        int n = int(v.size());
        for (int i = 0; i < n; i++) {
            int l, r, value;
            tie(l, r, value) = v[i];

            if (l <= key && key <= r)
                op(value);
        }
    }

    void queryRev(int key, const function<void(int)>& op) {
        int idx = key / keyBlockSize;

        auto& v = cmdBlock[idx];
        int n = int(v.size());
        for (int i = n - 1; i >= 0; i--) {
            int l, r, value;
            tie(l, r, value) = v[i];

            if (l <= key && key <= r)
                op(value);
        }
    }
};
