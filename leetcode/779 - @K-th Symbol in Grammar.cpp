// O(logK)
class Solution {
public:
    int kthGrammar(int N, int K) {
        int ans = 0;

        K--;
        while (K) {
            ans ^= K & 1;
            K /= 2;
        }

        return ans;
    }
};

// O(1)
class Solution {
public:
    int kthGrammar(int N, int K) {
        return __builtin_popcount(K - 1) & 1;
    }
};
