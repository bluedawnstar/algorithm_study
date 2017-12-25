#pragma once

template <typename T>
struct CycleDetection {
    // return (starting position, length)
    // If starting position is N, there is no cycle.
    static pair<int,int> floyd(function<T(T)> f, T x0) {
        T tortoise = f(x0);
        T hare = f(f(x0));

        while (tortoise != hare) {
            tortoise = f(tortoise);
            hare = f(f(hare));
        }

        int start = 0;
        tortoise = x0;
        while (tortoise != hare) {
            tortoise = f(tortoise);
            hare = f(hare);
            start++;
        }

        int length = 1;
        hare = f(tortoise);
        while (tortoise != hare) {
            hare = f(hare);
            length++;
        }

        return make_pair(start, length);
    }

    // return (starting position, length)
    // If starting position is N, there is no cycle.
    static pair<int,int> brent(function<T(T)> f, T x0) {
        int power = 1;
        int length = 1;

        T tortoise = x0;
        T hare = f(x0);

        while (tortoise != hare) {
            if (power == length) {
                tortoise = hare;
                power <<= 1;
                length = 0;
            }
            hare = f(hare);
            length++;
        }

        int start = 0;

        tortoise = x0;
        hare = x0;
        for (int i = 0; i < length; i++)
            hare = f(hare);

        while (tortoise != hare) {
            tortoise = f(tortoise);
            hare = f(hare);
            start++;
        }

        return make_pair(start, length);
    }
};
