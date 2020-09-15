#pragma once

// Prouhet-Tarry-Escott problem
struct ProuhetTarryEscottProblem {
    // Thue-Morse sequence (https://en.wikipedia.org/wiki/Thue%E2%80%93Morse_sequence)
    // result = [0, ..., 2^(k+1) - 1]
    // ex) k = 3, { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 } <- bitCount(i) % 2
    static vector<bool> generateThueMorseSequence(int k) {
        vector<bool> res(1 << (k + 1));

#if 0
        bool value = false;
        for (int i = 0, size = 1 << (k + 1); i < size; i++) {
            int x = i ^ (i - 1);
            if ((x ^ (x >> 1)) & 0x55555555)
                value = !value;
            res[i] = value;
        }
#else
        for (int i = 0, size = 1 << (k + 1); i < size; i++)
            res[i] = bool(popcount(i) & 1);
#endif

        return res;
    }

    // partition into two groups with the same sum in [(x + 0)^k, (x + 1)^k, (x + 2)^k, ..., (x + N - 1)^k ]
    // N = a multiple of 2^(k + 1)
    static vector<bool> partition(int N, int k) {
        vector<bool> seq = generateThueMorseSequence(k);

        vector<bool> res;
        res.reserve(N);
        for (int i = 0; i < N; i += 1 << (k + 1))
            res.insert(res.end(), seq.begin(), seq.end());  // repeat seq

        return res;
    }

private:
    // pop count
    static int popcount(int x) {
#ifndef __GNUC__
        return int(__popcnt(static_cast<unsigned>(x)));
#else
        return __builtin_popcount(static_cast<unsigned>(x));
#endif
    }
};