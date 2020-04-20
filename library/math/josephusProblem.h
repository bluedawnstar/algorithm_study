#pragma once

/*
    https://en.wikipedia.org/wiki/Josephus_problem#k=2

    Returns position of survivor among a circle of n persons and every second person being killed
    - Firstly, the person at position 1 is killed (not 0)

    Let f(n) be position of survivor for input n,

    1) n is even
        f(n) = 2 * f(n/2) - 1,             1 <= f(n) <= n


    2) n is odd
        f(n) = 2 * f((n-1)/2) + 1,         1 <= f(n) <= n

    =>

     f(n) = 2 * (n - 2^floor(log_2(n)) + 1
          = 2 * n - 2^(1 + floor(log_2(n))) + 1
*/
template <typename T>
struct JosephusProblemK2 {
    // 0 <= result < n, O(logN)
    static T getSafePosition(T n) {
        // find value of 2 ^ (1 + floor(log(n))) which is a power of 2 whose value is just above n. 
        T p = 1;
        while (p <= n)
            p <<= 1;
        return (n << 1) - p;
    }

    // 0 <= result < n, O(logN)
    static T getSafePosition2(T n) {
        T p = 1;
        while (p <= n)
            p <<= 1;
        return ~p & (n << 1);
    }
};

// https://cp-algorithms.com/others/josephus_problem.html
template <typename T>
struct JosephusProblem {
    // 0 <= result < n, O(K*logN)
    static T getSafePosition(T n, T k) {
        if (n == 1)
            return 0;
        if (k == 1)
            return n - 1;
        if (k > n)
            return (getSafePosition(n - 1, k) + k) % n;

        int cnt = n / k;
        int res = getSafePosition(n - cnt, k) - n % k;
        if (res < 0)
            res += n;
        else
            res += res / (k - 1);

        return res;
    }

    // 0 <= result < n, O(N)
    static T getSafePositionNaive(T n, T k) {
        T res = 0;
        for (int i = 1; i <= n; i++)
            res = (res + k) % i;
        return res;
    }
};
