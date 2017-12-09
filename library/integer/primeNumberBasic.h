#pragma once

// check if x is a prime number
inline bool isPrimeNumber(int x) {
    if (x < 2)
        return false;
    else if (x == 2)
        return true;
    else if ((x & 1) == 0)
        return false;

    int root = int(sqrt(x));
    for (int i = 3; i <= root; i += 2) {
        if (x % i == 0)
            return false;
    }

    return true;
}

// return all prime numbers from 1 to n
inline vector<int> findPrimeNumbers(int n) {
    vector<int> res;
    if (n < 2)
        return res;

    if (n >= 2)
        res.push_back(2);

    for (int i = 3; i <= n; i += 2) {
        bool isPrime = true;
        int root = int(sqrt(i));
        for (int j = 0; res[j] <= root; j++) {
            if (i % res[j] == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            res.push_back(i);
    }

    return res;
}


// return prime factors of x
inline vector<int> getPrimeFactors(int x) {
    vector<int> res;

    int root = int(sqrt(x));
    if (root >= 2) {
        while (x % 2 == 0) {
            x /= 2;
            res.push_back(2);
        }
    }

    for (int i = 3; i <= root; i += 2) {
        while (x % i == 0) {
            x /= i;
            res.push_back(i);
        }
    }

    if (x > 1)
        res.push_back(x);

    return res;
}

// return prime factor and power of x
inline vector<pair<int, int>> getPrimeFactors2(int x) {
    vector<pair<int, int>> res;

    int root = int(sqrt(x));
    if (root >= 2) {
        int cnt = 0;
        while (x % 2 == 0) {
            x /= 2;
            cnt++;
        }
        if (cnt > 0)
            res.push_back(make_pair(2, cnt));
    }

    for (int i = 3; i <= root; i += 2) {
        int cnt = 0;
        while (x % i == 0) {
            x /= i;
            cnt++;
        }
        if (cnt > 0)
            res.push_back(make_pair(i, cnt));
    }

    if (x > 1)
        res.push_back(make_pair(x, 1));

    return res;
}

// return all prime numbers and prime factors from 1 to n
inline void getPrimeFactors(int n, vector<int>& primes,
    vector<vector<pair<int, int>>>& primeFactors) {
    primes.clear();
    primeFactors = vector<vector<pair<int, int>>>(n + 1);
    if (n < 2)
        return;

    if (n >= 2) {
        primes.push_back(2);
        primeFactors[2].push_back(make_pair(2, 1));
    }

    for (int i = 3; i <= n; i++) {
        bool isPrime = true;
        int root = int(sqrt(i));
        for (int j = 0; primes[j] <= root; j++) {
            if (i % primes[j] == 0) {
                primeFactors[i] = primeFactors[i / primes[j]];
                if (primeFactors[i][0].first == primes[j])
                    primeFactors[i][0].second++;
                else
                    primeFactors[i].insert(primeFactors[i].begin(),
                        make_pair(primes[j], 1));
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(i);
            primeFactors[i].push_back(make_pair(i, 1));
        }
    }
}
