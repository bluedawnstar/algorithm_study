#pragma once

// Wythoff's game
// https://en.wikipedia.org/wiki/Wythoff%27s_game
// https://math.rice.edu/~michael/teaching/2012Fall/Wythoff.pdf
struct WythoffGame {
    // Using Beatty Theorem, O(1)
    //   
    //   R = golden ratio (1+sqrt(5))/2 or (1-sqrt(5))/2
    //   R^2 - R - 1 = 0
    //   <=> R^2 = R + 1
    //   <=> 1/R + 1/R^2 = 1
    //   <=> 1/R + 1/(R + 1) = 1

    // n >= 0
    static pair<int,int> find(int n) {
        static const double R = (1.0 + sqrt(5.0)) / 2;

        pair<int, int> res;
        if (n > 0) {
            res.first = int(R * n);
            res.second = res.first + n;
        }
        return res;
    }

    // n >= 0
    static pair<long long, long long> findLL(int n) {
        static const double R = (1.0 + sqrt(5.0)) / 2;

        pair<long long, long long> res;
        if (n > 0) {
            res.first = (long long)(R * n);
            res.second = res.first + n;
        }
        return res;
    }
};
