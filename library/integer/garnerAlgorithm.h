#pragma once

#include "chineseRemainderTheorem.h"
#include "bigint.h"

/*
https://e-maxx-eng.appspot.com/algebra/chinese-remainder-theorem.html
 
Garner Algorithm (x10 faster than bigint)

1. Formula
   x = a0 + a1 * p1 + a2 * p1 * p2 + ... + an * p1 * p2 * ... * pn

2. Coverage
   { x | x < 10^450 }, N = 100
   { x | x < 10^900 }, N = 200
*/
struct GarnerBigInt {
    static constexpr int N = 200;
    const int* P;
    const int (*R)[N];

    vector<int> A;

    GarnerBigInt() : A(N) {
        makeTable();
    }

    GarnerBigInt(int x) : A(N) {
        makeTable();
        for (int i = 0; i < N; i++)
            A[i] = x % P[i];
    }

    GarnerBigInt(const bigint& x) : A(N) {
        makeTable();
        for (int i = 0; i < N; i++)
            A[i] = x % P[i];
    }

    GarnerBigInt operator +(const GarnerBigInt& x) const {
        GarnerBigInt res;
        for (int i = 0; i < N; ++i)
            res.A[i] = (A[i] + x.A[i]) % P[i];
        return res;
    }

    GarnerBigInt& operator +=(const GarnerBigInt& x) {
        for (int i = 0; i < N; i++)
            A[i] = (A[i] + x.A[i]) % P[i];
        return *this;
    }

    GarnerBigInt operator -(const GarnerBigInt& x) const {
        GarnerBigInt res;
        for (int i = 0; i < N; i++)
            res.A[i] = (A[i] - x.A[i] + P[i]) % P[i];
        return res;
    }

    GarnerBigInt& operator -=(const GarnerBigInt& x) {
        for (int i = 0; i < N; i++)
            A[i] = (A[i] - x.A[i] + P[i]) % P[i];
        return *this;
    }

    GarnerBigInt operator *(const GarnerBigInt& x) const {
        GarnerBigInt res;
        for (int i = 0; i < N; i++)
            res.A[i] = int((1ll * A[i] * x.A[i]) % P[i]);
        return res;
    }

    GarnerBigInt& operator *=(const GarnerBigInt& x) {
        for (int i = 0; i < N; i++)
            A[i] = int((1ll * A[i] * x.A[i]) % P[i]);
        return *this;
    }

    bigint get(bool canBeNegative = false) {
        bigint res(0), mult(1);
        vector<int> x(N);
        for (int i = 0; i < N; i++) {
            x[i] = A[i];
            for (int j = 0; j < i; j++) {
                long long cur = 1ll * (x[i] - x[j]) * R[j][i];
                x[i] = int((cur % P[i] + P[i]) % P[i]);
            }
            res += mult * x[i];
            mult *= P[i];
        }

        if (canBeNegative) {
            mult /= 2;
            if (res >= mult)
                res -= mult;
        }

        return res;
    }

private:
    void makeTable(void) {
        static bool initialized = false;
        static int primes[N] = {
            // 0..99
            1000000007, 1000000009, 1000000021, 1000000033, 1000000087, 1000000093, 1000000097, 1000000103, 1000000123, 1000000181,
            1000000207, 1000000223, 1000000241, 1000000271, 1000000289, 1000000297, 1000000321, 1000000349, 1000000363, 1000000403,
            1000000409, 1000000411, 1000000427, 1000000433, 1000000439, 1000000447, 1000000453, 1000000459, 1000000483, 1000000513,
            1000000531, 1000000579, 1000000607, 1000000613, 1000000637, 1000000663, 1000000711, 1000000753, 1000000787, 1000000801,
            1000000829, 1000000861, 1000000871, 1000000891, 1000000901, 1000000919, 1000000931, 1000000933, 1000000993, 1000001011,
            1000001021, 1000001053, 1000001087, 1000001099, 1000001137, 1000001161, 1000001203, 1000001213, 1000001237, 1000001263,
            1000001269, 1000001273, 1000001279, 1000001311, 1000001329, 1000001333, 1000001351, 1000001371, 1000001393, 1000001413,
            1000001447, 1000001449, 1000001491, 1000001501, 1000001531, 1000001537, 1000001539, 1000001581, 1000001617, 1000001621,
            1000001633, 1000001647, 1000001663, 1000001677, 1000001699, 1000001759, 1000001773, 1000001789, 1000001791, 1000001801,
            1000001803, 1000001819, 1000001857, 1000001887, 1000001917, 1000001927, 1000001957, 1000001963, 1000001969, 1000002043,
            // 100..199
            1000002089, 1000002103, 1000002139, 1000002149, 1000002161, 1000002173, 1000002187, 1000002193, 1000002233, 1000002239,
            1000002277, 1000002307, 1000002359, 1000002361, 1000002431, 1000002449, 1000002457, 1000002499, 1000002571, 1000002581,
            1000002607, 1000002631, 1000002637, 1000002649, 1000002667, 1000002727, 1000002791, 1000002803, 1000002821, 1000002823,
            1000002827, 1000002907, 1000002937, 1000002989, 1000003009, 1000003013, 1000003051, 1000003057, 1000003097, 1000003111,
            1000003133, 1000003153, 1000003157, 1000003163, 1000003211, 1000003241, 1000003247, 1000003253, 1000003267, 1000003271,
            1000003273, 1000003283, 1000003309, 1000003337, 1000003351, 1000003357, 1000003373, 1000003379, 1000003397, 1000003469,
            1000003471, 1000003513, 1000003519, 1000003559, 1000003577, 1000003579, 1000003601, 1000003621, 1000003643, 1000003651,
            1000003663, 1000003679, 1000003709, 1000003747, 1000003751, 1000003769, 1000003777, 1000003787, 1000003793, 1000003843,
            1000003853, 1000003871, 1000003889, 1000003891, 1000003909, 1000003919, 1000003931, 1000003951, 1000003957, 1000003967,
            1000003987, 1000003999, 1000004023, 1000004059, 1000004099, 1000004119, 1000004123, 1000004207, 1000004233, 1000004249,
        };
        static int invPrimes[N][N];

        if (!initialized) {
            for (int i = 0; i < N; i++)
                for (int j = i + 1; j < N; j++)
                    invPrimes[i][j] = modInv(primes[i], primes[j]);
            initialized = true;
        }
        P = primes;
        R = invPrimes;
    }
};
