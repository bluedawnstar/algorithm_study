#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

// Bit-String Longest Common Subsequence (LCS) Algorithm
// 
// http://www.csse.monash.edu.au/~lloyd/tildeStrings/Alignment/86.IPL.html
// https://github.com/zimpha/algorithmic-library/blob/master/StringUtility/bit-lcs.cc

struct BitLCS {
    struct Bitset {
        typedef long long ValueT;
        static const int BIT_SIZE = 62;

        int N;
        int valueN;
        vector<ValueT> bits;

        Bitset() : N(0), valueN(0) {
        }

        explicit Bitset(int n) {
            init(n);
        }

        void init(int n) {
            N = n;
            valueN = (n + BIT_SIZE - 1) / BIT_SIZE;
            bits.assign(valueN, 0);
        }

        void clear() {
            memset(bits.data(), 0, sizeof(ValueT) * bits.size());
        }

        void set(int x) {
            bits[x / BIT_SIZE] |= 1ll << (x % BIT_SIZE);
        }

        Bitset operator |(const Bitset& r) const {
            Bitset s(N);
            for (int i = 0; i < valueN; i++)
                s.bits[i] = bits[i] | r.bits[i];
            return s;
        }

        // a = bit_sub(b, bit_and(a, b))
        void replace(const Bitset& r) {
            for (int i = 0; i < valueN; i++) {
                bits[i] = (bits[i] ^ r.bits[i]) & r.bits[i];
            }
        }

        void shiftL() {
            ValueT c = 1;
            for (int i = 0; i < valueN; i++) {
                bits[i] <<= 1;
                bits[i] |= c;

                c = (bits[i] >> BIT_SIZE) & 1;
                bits[i] ^= c << BIT_SIZE;
            }
        }

        void subtract(const Bitset& r) {
            for (int i = 0; i < valueN; i++)
                bits[i] = r.bits[i] - bits[i];

            for (int i = 0; i < valueN; i++) {
                if (bits[i] < 0) {
                    bits[i] += 1ll << BIT_SIZE;
                    if (i + 1 < valueN)
                        bits[i + 1]--;
                }
            }
        }

        int bitCount() const {
            int res = 0;

            for (int i = 0; i < valueN; i++) {
#if defined(_M_X64)
                res += int(__popcnt64((unsigned long long)bits[i]));
#elif defined(__GNUC__)
                res += __builtin_popcountll((unsigned long long)bits[i]);
#else
                res += int(__popcnt(unsigned(bits[i]))) + int(__popcnt(unsigned((unsigned long long)bits[i] >> 32)));
#endif
            }

            return res;
        }
    };

    static const int MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }

    // O((|S|/W)*|T|), W = 62
    // PRECONDITION: |S| >= |T|
    static int solve(const string& S, const string& T) {
        int N = int(S.length());
        int M = int(T.length());

        vector<Bitset> alphaS(MaxCharN);
        for (int i = 0; i < MaxCharN; i++)
            alphaS[i].init(N);

        for (int i = 0; i < N; i++)
            alphaS[ch2i(S[i])].set(i);

        int valueN = (N + Bitset::BIT_SIZE - 1) / Bitset::BIT_SIZE;

        int res = 0;

        Bitset row(N), x(N);
        for (int i = 0; i < M; i++) {
            int ch = ch2i(T[i]);
            for (int j = 0; j < valueN; j++)
                x.bits[j] = row.bits[j] | alphaS[ch].bits[j];

            row.shiftL();
            row.subtract(x);
            row.replace(x);

            res = max(res, row.bitCount());
        }

        return res;
    }
};
