#pragma once

// https://www.codechef.com/problems/MDSWIN
// -> https://discuss.codechef.com/t/mdswin-editorial/44120


/*
    X = a + b*w
    An algebraic extended number system, where w^3 = 1

        w^3 = 1 <=> (w - 1) * (w^2 + w + 1) = 0
    ==> w^2 + w + 1 = 0
    <=> w*w = -1 - w
*/
template <typename T, int mod = 1000000007>
struct NumW3Mod : public pair<T,T> {
    static T normalize(T x) {
        if (x < 0)
            x += mod;
        else if (x >= mod)
            x -= mod;
        return x;
    }

    template <typename U>
    static T normalizeL(U x) {
        T res = T(x % mod);
        if (res < 0)
            res += mod;
        else if (res >= mod)
            res -= mod;
        return res;
    }

    //---

    NumW3Mod() : pair<T, T>(0, 0) {
    }

    NumW3Mod(T a, T b) : pair<T, T>(a, b) {
    }

    NumW3Mod operator +(const NumW3Mod& rhs) const {
        return NumW3Mod(normalize(this->first + rhs.first),
                        normalize(this->second + rhs.second));
    }

    NumW3Mod& operator +=(const NumW3Mod& rhs) {
        this->first = normalize(this->first + rhs.first);
        this->second = normalize(this->second + rhs.second);
        return *this;
    }

    template <typename U>
    NumW3Mod operator *(U x) const {
        return NumW3Mod(normalizeL(1ll * this->first * x),
                        normalizeL(1ll * this->second * x));
    }

    template <typename U>
    NumW3Mod& operator *=(U x) {
        this->first = normalizeL(1ll * this->first * x);
        this->second = normalizeL(1ll * this->second * x);
        return *this;
    }

    NumW3Mod operator *(const NumW3Mod& rhs) const {
        return NumW3Mod(normalizeL(1ll * this->first * rhs.first - 1ll * this->second * rhs.second),
                        normalizeL(1ll * this->first * rhs.second + 1ll * this->second * rhs.first - 1ll * this->second * rhs.second));
    }

    NumW3Mod& operator *=(const NumW3Mod& rhs) {
        auto a = normalizeL(1ll * this->first * rhs.first - 1ll * this->second * rhs.second);
        auto b = normalizeL(1ll * this->first * rhs.second + 1ll * this->second * rhs.first - 1ll * this->second * rhs.second);
        this->first = a;
        this->second = b;
        return *this;
    }

    // pow(x, n) = x ^ n
    template <typename U>
    NumW3Mod operator ^(U n) const {
        auto x = *this;

        NumW3Mod res(T(1), T(0));
        while (n) {
            if (n & 1)
                res *= x;
            x *= x;
            n >>= 1;
        }

        return res;
    }
};

template <typename T, int mod = 1000000007>
struct FWHTMod3Xor {
    // converts the binary representation of x to ternary
    // - ex: 5 (101_b2) => 10 (101_b3)
    static T binaryToBase3(T x) {
        T res = 0;
        for (T scale = 1; x; scale *= 3, x >>= 1) {
            if (x & 1)
                res += scale;
        }
        return res;
    }

    // C = SUM SUM A[i] * B[j] * x^(i 3xor j)  ---  i and j are numbers in bases
    //      i   j
    // A = a list of count of numbers in base 3
    // B = a list of count of numbers in base 3
    // O(N*log_3(N))
    static vector<T> multiply(const vector<T>& A, const vector<T>& B) {
        int size = 1;
        while (size < int(A.size()) || size < int(B.size()))
            size *= 3;

        vector<NumW3Mod<T,mod>> tA(size);
        vector<NumW3Mod<T,mod>> tB(size);
        for (int i = 0; i < int(A.size()); i++)
            tA[i].first = A[i];
        for (int i = 0; i < int(B.size()); i++)
            tB[i].first = B[i];

        transform(tA, false);
        transform(tB, false);

        for (int i = 0; i < size; i++)
            tA[i] *= tB[i];

        transform(tA, true);

        vector<T> res(size);
        for (int i = 0; i < size; i++)
            res[i] = tA[i].first;

        return res;
    }

    // C = { A[0] + A[1] * x^1 + A[2] * x^3 + ... + A[n-1] * x^(n-1) } ^ k
    //     A_square[i 3xor j] += A[i] * A[j] * x^(i 3xor j)
    // A = a list of count of numbers in base 3
    // O(N*log_3(N))
    template <typename U>
    static vector<T> pow(const vector<T>& A, U k) {
        int size = 1;
        while (size < int(A.size()))
            size *= 3;

        vector<NumW3Mod<T,mod>> tA(size);
        for (int i = 0; i < int(A.size()); i++)
            tA[i].first = A[i];

        transform(tA, false);

        for (int i = 0; i < size; i++)
            tA[i] = tA[i] ^ k;

        transform(tA, true);

        vector<T> res(size);
        for (int i = 0; i < size; i++)
            res[i] = tA[i].first;

        return res;
    }

private:
    // the size of P must be a power of 3
    static void transform(vector<NumW3Mod<T,mod>>& P, bool inverse) {
        static NumW3Mod<T, mod> w(0, 1);
        static NumW3Mod<T, mod> w2(mod - 1, mod - 1);

        int N = int(P.size());
        for (int len = 1; len < N; len *= 3) {
            int pitch = len * 3;
            int len2 = len * 2;
            for (int i = 0; i < N; i += pitch) {
                for (int j = 0; j < len; j++) {
                    auto a = P[i + j       ];
                    auto b = P[i + j + len ];
                    auto c = P[i + j + len2];
                    P[i + j       ] = a + b      + c     ;
                    P[i + j + len ] = a + b * w  + c * w2;
                    P[i + j + len2] = a + b * w2 + c * w ;
                    if (inverse)
                        swap(P[i + j + len], P[i + j + len2]);
                }
            }
        }

        if (inverse) {
            long long inv3 = (mod + 1) / 3;
            long long inv = 1;
            for (int i = 1; i < N; i *= 3)
                inv = inv3 * inv % mod;

            for (int i = 0; i < N; i++)
                P[i] *= inv;
        }
    }
};
