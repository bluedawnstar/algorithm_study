#pragma once

// https://www.codechef.com/MAY20A/problems/TWOSTRS

#include "../string/ahoCorasickPatternWeightSum.h"

/*
   - pattern[i] = { string, ... }, all patterns are pairwise distinct
   - weight[i] = weight of pattern[i]
   - 1 <= pattern[i] <= maxPatternLength

   find max weight-sum of found patterns on all concatenated strings A[...i] + B[j...]
      -1 <= i < length(A),  -1 means A not to be used
      -1 <= j < length(B),  -1 means B not to be used
*/
struct MaxWeightOfAllConcatenatedStrings2 {
    // O(sum(|patterns|) + |A|*|B|*max(|patterns|))
    static long long solve(const string& A, const string& B,
            const vector<string>& patterns, const vector<int>& weights,
            int maxPatternLength) {

        AhoCorasickPatternWeightSum<> aho;

        for (int i = 0; i < int(patterns.size()); i++)
            aho.insert(patterns[i], weights[i]);
        aho.build();

        int nA = int(A.length());
        int nB = int(B.length());

        vector<long long> dpA(nA + 1);
        vector<long long> dpB(nB + 1);

        vector<const AhoCorasickPatternWeightSum<>::Node*> nodeA(nA + 1);
        nodeA[0] = &aho.mRoot;
        {
            long long w = 0;
            for (int i = 1; i <= nA; i++) {
                nodeA[i] = aho.nextAhoCorasick(nodeA[i - 1], A[i - 1], w);
                dpA[i] = w;
            }
        }

        {
            const AhoCorasickPatternWeightSum<>::Node* p = &aho.mRoot;
            for (int i = 0; i < nB; i++)
                p = aho.nextAhoCorasick(p, B[i], dpB[i]);
            for (int i = nB - 1; i >= 0; i--)
                dpB[i] += dpB[i + 1];
        }

        //--- find maximum weight

        long long res = 0;
        for (int i = 0; i <= nA; i++) {
            for (int j = 0; j <= nB; j++) {
                /*
                   A[..:i] | B[j:..]

                 #..........x
                  #..........x
                      . . .
                          #..........x
                */

                long long w = 0;

                auto* p = nodeA[i];
                int k;
                for (k = 0; k + 1 < maxPatternLength && j + k < nB; k++)
                    p = aho.nextAhoCorasick(p, B[j + k], w);

                res = max(res, w + dpA[i] + dpB[j + k]);
            }
        }

        return res;
    }
};
