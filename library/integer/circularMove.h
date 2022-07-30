#pragma once

// https://www.hackerearth.com/problem/algorithm/modulus-of-grid-b690bf11/

struct CircularMove {
    vector<vector<int>> factors;
    vector<int> phi;                // Euler's phi() function

    CircularMove() {
    }

    explicit CircularMove(int maxN) {
        prepare(maxN);
    }

    // O(N*loglogN)
    void prepare(int maxN) {
        factors = vector<vector<int>>(maxN + 1, vector<int>{1});

        phi.resize(maxN + 1);
        iota(phi.begin(), phi.end(), 0);
        phi[0] = 0;
        phi[1] = 0;
        for (int i = 2; i <= maxN; i++) {
            for (int j = i; j <= maxN; j += i)
                factors[j].push_back(i);

            if (phi[i] == i) {
                phi[i] = i - 1;
                for (int j = 2 * i; j <= maxN; j += i)
                    phi[j] = (phi[j] / i) * (i - 1);
            }
        }
    }

    // jump_step * c + srcX = dstX (mod N) ,   (1 <= jump_step < N, 0 <= c,  0 <= srcX, dstX < N)
    // return the number of jump_step

    //                     jump_step * c = N * a + dstX - srcX
    // <=>                 jump_step * c = N * a + abs(dstX - srcX) * b
    // <=> gcd(N, abs(dstX - srcX)) * c' = N * a + abs(dstX - srcX) * b
    // 
    // ==> jump_step has factors of gcd(N, abs(dstX - srcX)) including 1

    // O(loglog(N))
    int countJumpSteps(int srcX, int dstX, int N) const {
        int res = 0;

        if (srcX == dstX) {
            res = N - 1;        // any jump_step is possible with a == 0
        } else {
            int dx = dstX - srcX;

            int g = gcd(N, abs(dx));
            for (auto f : factors[g])
                res += phi[N / f];
        }

        return res;
    }

private:
    static int gcd(int p, int q) {
        return q == 0 ? p : gcd(q, p % q);
    }

    // O(sqrt(x))
    static vector<int> getFactors(int x) {
        vector<int> res;
        if (x <= 0)
            return res;

        int i;
        for (i = 1; i * i < x; i++) {
            if (x % i == 0) {
                res.push_back(i);
                res.push_back(x / i);
            }
        }

        if (i * i == x)
            res.push_back(i);

        sort(res.begin(), res.end());

        return res;
    }
};
