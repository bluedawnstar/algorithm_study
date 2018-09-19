class Solution {
public:
    enum { MOD = 1000000007 };

    int checkRecord(int n) {
        if (n <= 0)
            return 0;
        else if (n == 1)
            return 3;

        long long xP[2], xA[2], xL[2], xLL[2];
        long long AP[2], AL[2], ALL[2];

        int from = 0, to = 1;

        xP[from] = 1;
        xA[from] = 1;
        xL[from] = 1;
        xLL[from] = 0;

        AP[from] = 0;
        AL[from] = 0;
        ALL[from] = 0;
        for (int i = 1; i < n; i++) {
            xP[to] = (xP[from] + xL[from] + xLL[from]) % MOD;               // P
            xA[to] = (xP[from] + xL[from] + xLL[from]) % MOD;               // A
            xL[to] = xP[from];                                              // L
            xLL[to] = xL[from];                                             // L

            AP[to] = (xA[from] + AP[from] + AL[from] + ALL[from]) % MOD;    // P
            AL[to] = (xA[from] + AP[from]) % MOD;                           // L
            ALL[to] = AL[from];                                             // L

            swap(from, to);
        }

        long long A0 = (xP[from] + xL[from] + xLL[from]);
        long long A1 = (xA[from] + AP[from] + AL[from] + ALL[from]);

        return int((A0 + A1) % MOD);
    }
};
