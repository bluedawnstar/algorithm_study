#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

// https://www.codechef.com/problems/GUESSPRM

struct PrimeModulusFinder {
    static const int MAXP = 1000000000;
    static const int P1 = 31627;    // a small prime number that satisfies P1 * P1 > MAXP

    vector<int> primeFactors;

    explicit PrimeModulusFinder(int R1) {
        primeFactors = getPrimeFactors(1ll * P1 * P1 - R1);
    }

    int findQuestion() const {
        int P2 = 1;
        for (;; P2++) {
            unordered_set<int> S;
            for (auto f : primeFactors)
                S.insert(int(1ll * P2 * P2 % f));
            if (S.size() == primeFactors.size())
                break;
        }
        return P2;
    }

    int findAnswer(int P2, int R2) const {
        int res = primeFactors.back();
        for (auto f : primeFactors) {
            if (1ll * P2 * P2 % f == R2) {
                res = f;
                break;
            }
        }
        return res;
    }

private:
    static vector<int> getPrimeFactors(long long x) {
        vector<int> res;

        int root = int(sqrt(x));
        if (root >= 2) {
            int n = 0;
            while (x % 2 == 0) {
                x /= 2;
                n++;
            }
            if (n > 0) {
                res.emplace_back(2);
            }
        }

        for (int i = 3; i <= root; i += 2) {
            int n = 0;
            while (x % i == 0) {
                x /= i;
                n++;
            }
            if (n > 0)
                res.emplace_back(i);
        }

        if (x > 1 && x <= MAXP)
            res.emplace_back(int(x));

        sort(res.begin(), res.end());

        return res;
    }
};

static int solver() {
    int P2, R1, R2;

    cout << "1 " << PrimeModulusFinder::P1 << endl;
    cin >> R1;

    PrimeModulusFinder solver(R1);

    P2 = solver.findQuestion();
    cout << "1 " << P2 << endl;
    cin >> R2;

    return solver.findAnswer(P2, R2);
}
