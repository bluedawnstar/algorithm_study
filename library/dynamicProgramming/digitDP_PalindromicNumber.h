#pragma once

// https://www.hackerearth.com/challenges/competitive/february-circuits-20/algorithm/kth-palindrome-45f48699/ 

struct DigitDP_PalindromicNumber {
    static long long count(long long n) {
        if (n < 1ll)
            return 0ll;
        else if (n < 10ll)
            return n;

        long long res = 0ll;

        static long long totalCount[] = {
            0ll,
            9ll,                // [10^0, 10^1)
            9ll,                // [10^1, 10^2)
            90ll,               // [10^2, 10^3)
            90ll,               // [10^3, 10^4)
            900ll,              // [10^4, 10^5)
            900ll,              // [10^5, 10^6)
            9000ll,             // [10^6, 10^7)
            9000ll,             // [10^7, 10^8)
            90000ll,            // [10^8, 10^9)
            90000ll,            // [10^9, 10^10)
            900000ll,           // [10^10, 10^11)
            900000ll,           // [10^11, 10^12)
            9000000ll,          // [10^12, 10^13)
            9000000ll,          // [10^13, 10^14)
            90000000ll,         // [10^14, 10^15)
            90000000ll,         // [10^15, 10^16)
            900000000ll,        // [10^16, 10^17)
            900000000ll,        // [10^17, 10^18)
        };

        int logScale = getLogScale(n);
        for (int i = 1; i < logScale; i++)
            res += totalCount[i];

        long long scale = pow10(logScale / 2);
        if (logScale & 1) {
            res += n / scale - scale;       // scale = ["000...001"0, "099...999"9] + 1(tight)
            if (reverse((n / scale) / 10) <= (n % scale))
                res++;
        } else {
            res += n / scale - scale / 10;  // scale / 10 = ["000...001" ~ "099...999"] + 1(tight)
            if (reverse(n / scale) <= (n % scale))
                res++;
        }

        return res;
    }

    long long kth(long long k) {
        long long lo = 1, hi = static_cast<long long>(1e18);
        while (lo <= hi) {
            long long mid = lo + (hi - lo) / 2;
            if (count(mid) >= k)
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        return lo;
    }

private:
    static int getLogScale(long long x) {
        int res = 0;
        while (x) {
            res++;
            x /= 10;
        }
        return res;
    }

    static long long pow10(int x) {
        long long res = 1;
        while (x-- > 0)
            res *= 10;
        return res;
    }

    static long long reverse(long long x) {
        long long res = 0;
        while (x) {
            res = res * 10 + x % 10;
            x /= 10;
        }
        return res;
    }
};
