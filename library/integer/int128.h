#pragma once

#ifdef __GNUC__
//#include <x86intrin.h>
#include <immintrin.h>

typedef __int128_t      Int128;
typedef __uint128_t     UInt128;

inline int clz(unsigned int x) {
    return __builtin_clz(x);
}

inline int ctz(unsigned int x) {
    return __builtin_ctz(x);
}

inline int popcount(unsigned int x) {
    return __builtin_popcount(x);
}

inline int clz(unsigned long long x) {
    return __builtin_clzll(x);
}

inline int ctz(unsigned long long x) {
    return __builtin_ctzll(x);
}

inline int popcount(unsigned long long x) {
    return __builtin_popcountll(x);
}

inline int clz(__int128_t x) {
    return __builtin_clzll(static_cast<unsigned long long>(x >> 64))
         + __builtin_clzll(static_cast<unsigned long long>(x      ));
}

inline int ctz(__int128_t x) {
    return __builtin_ctzll(static_cast<unsigned long long>(x >> 64))
         + __builtin_ctzll(static_cast<unsigned long long>(x      ));
}

inline int popcount(__int128_t x) {
    return __builtin_popcountll(static_cast<unsigned long long>(x >> 64))
         + __builtin_popcountll(static_cast<unsigned long long>(x      ));
}

inline int clz(__uint128_t x) {
    return __builtin_clzll(static_cast<unsigned long long>(x >> 64))
         + __builtin_clzll(static_cast<unsigned long long>(x      ));
}

inline int ctz(__uint128_t x) {
    return __builtin_ctzll(static_cast<unsigned long long>(x >> 64))
         + __builtin_ctzll(static_cast<unsigned long long>(x      ));
}

inline int popcount(__uint128_t x) {
    return __builtin_popcountll(static_cast<unsigned long long>(x >> 64))
         + __builtin_popcountll(static_cast<unsigned long long>(x      ));
}


inline Int128 makeInt128(long long hi, unsigned long long lo) {
    return (__int128_t(hi) << 64) | lo;
}

inline UInt128 makeUInt128(unsigned long long hi, unsigned long long lo) {
    return (__uint128_t(hi) << 64) | lo;
}

#else

#include <intrin.h>
#include <immintrin.h>

#pragma warning(disable: 4146)

inline int clz(unsigned int x) {
    return int(_lzcnt_u32(x));
}

inline int ctz(unsigned int x) {
    return int(_tzcnt_u32(x));
}

inline int popcount(unsigned int x) {
    return int(__popcnt(x));
}

inline int clz(unsigned long long x) {
#if defined(_M_X64)
    return int(_lzcnt_u64(x));
#else
    if ((x >> 32) != 0)
        return int(_lzcnt_u32(unsigned(x >> 32)));
    else
        return 32 + int(_lzcnt_u32(unsigned(x)));
#endif
}

inline int ctz(unsigned long long x) {
#if defined(_M_X64)
    return int(_tzcnt_u64(x));
#else
    if ((x >> 32) != 0)
        return int(_tzcnt_u32(unsigned(x >> 32)));
    else
        return 32 + int(_tzcnt_u32(unsigned(x)));
#endif
}

inline int popcount(unsigned long long x) {
#if defined(_M_X64)
    return int(__popcnt64(x));
#else
    return int(__popcnt(unsigned(x))) + int(__popcnt(unsigned(x >> 32)));
#endif
}

inline unsigned long long mulx_u64_slow(unsigned long long x, unsigned long long y, unsigned long long* hi) {
    unsigned long long xl = x & 0xffffffff;
    unsigned long long xh = x >> 32;
    unsigned long long yl = y & 0xffffffff;
    unsigned long long yh = y >> 32;

    unsigned long long t = xl * yl;
    unsigned long long l = t & 0xffffffff;
    unsigned long long h = t >> 32;

    t = xh * yl;
    t += h;
    h = t >> 32;

    t = xl * yh + (t & 0xffffffff);
    l |= t << 32;
    *hi = xh * yh + h + (t >> 32);

    return l;
}
#ifdef __GNUC__
#define _mulx_u64   mulx_u64_slow
#endif

// (hi, lo)
inline
pair<pair<unsigned long long, unsigned long long>,  // quotient
     pair<unsigned long long, unsigned long long>>  // remainder
udivmod(pair<unsigned long long, unsigned long long> num,
        pair<unsigned long long, unsigned long long> den) {
    pair<unsigned long long, unsigned long long> bit(0ull, 1ull);
    pair<unsigned long long, unsigned long long> res(0ull, 0ull);

    while ((den < num) && (bit.first || bit.second) && !(den.first >> 63)) {
        den.first = (den.first << 1) | (den.second >> 63);      // den <<= 1
        den.second <<= 1;
        bit.first = (bit.first << 1) | (bit.second >> 63);      // bit <<= 1
        bit.second <<= 1;
    }
    while (bit.first || bit.second) {
        if (num >= den) {
            num.first -= den.first + _subborrow_u64(0, num.second, den.second, &num.second);
            // num -= den
            res.first |= bit.first;                             // res |= bit
            res.second |= bit.second;
        }
        bit.second = (bit.first << 63) | (bit.second >> 1);     // bit >>= 1
        bit.first >>= 1;
        den.second = (den.first << 63) | (den.second >> 1);     // den >>= 1
        den.first >>= 1;
    }

    return make_pair(res, num);
}

// (hi, lo)
struct Int128 : pair<long long, unsigned long long> {
    Int128() : pair<long long, unsigned long long>() {
    }

    Int128(long long hi, unsigned long long lo) : pair<long long, unsigned long long>(hi, lo) {
    }

    Int128(unsigned long long hi, unsigned long long lo) : pair<long long, unsigned long long>(static_cast<long long>(hi), lo) {
    }

    template <typename T>
    Int128(T rhs) {
        operator =(rhs);
    }

    explicit Int128(const pair<long long, unsigned long long>& rhs) {
        operator =(rhs);
    }

    explicit Int128(const pair<unsigned long long, unsigned long long>& rhs) {
        operator =(rhs);
    }

    Int128(const Int128& rhs) {
        operator =(rhs);
    }


    Int128& operator =(const Int128& rhs) {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }

    Int128& operator =(long long rhs) {
        if (rhs < 0) {
            first = -1ll;
            second = static_cast<unsigned long long>(rhs);
        } else {
            first = 0;
            second = static_cast<unsigned long long>(rhs);
        }
        return *this;
    }

    Int128& operator =(unsigned long long rhs) {
        first = 0;
        second = rhs;
        return *this;
    }

    template <typename T>
    Int128& operator =(T rhs) {
        return operator =(static_cast<long long>(rhs));
    }

    Int128& operator =(const pair<long long, unsigned long long>& rhs) {
        first = rhs.first; 
        second = rhs.second;
        return *this;
    }

    Int128& operator =(const pair<unsigned long long, unsigned long long>& rhs) {
        first = static_cast<long long>(rhs.first);
        second = rhs.second;
        return *this;
    }


    operator bool() const {
        return first != 0 || second != 0;
    }

    template <typename T>
    operator T() const {
        return static_cast<T>(second);
    }


    Int128 operator -() const {
        return Int128(!second ? -first : ~first, !second ? 0ull : -second);
    }

    const Int128& operator +() const {
        return *this;
    }
    

    Int128 operator +(const Int128& rhs) const {
        Int128 res;
        res.first = first + rhs.first + _addcarry_u64(0, second, rhs.second, &res.second);
        return res;
    }

    Int128& operator +=(const Int128& rhs) {
        first += rhs.first + _addcarry_u64(0, second, rhs.second, &second);
        return *this;
    }

    Int128 operator -(const Int128& rhs) const {
        Int128 res;
        res.first = first - (rhs.first + _subborrow_u64(0, second, rhs.second, &res.second));
        return res;
    }

    Int128& operator -=(const Int128& rhs) {
        first -= rhs.first + _subborrow_u64(0, second, rhs.second, &second);
        return *this;
    }

    Int128 operator *(const Int128& rhs) const {
        Int128 res;
//#ifndef __GNUC__
//        long long t;
//        res.second = static_cast<unsigned long long>(_umul128(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
//        res.first += _mul128(second, rhs.first, &t);
//        res.first += _mul128(first, rhs.second, &t);
//#else    
        unsigned long long t;
        res.second = static_cast<unsigned long long>(_mulx_u64(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
        res.first += _mulx_u64(second, static_cast<unsigned long long>(rhs.first), &t);
        res.first += _mulx_u64(static_cast<unsigned long long>(first), rhs.second, &t);
//#endif
        return res;
    }

    Int128& operator *=(const Int128& rhs) {
        auto t = operator *(rhs);
        return *this = t;
    }

    //---

    // (quotient, remainder)
    pair<Int128, Int128> divmod(const Int128& rhs) const {
        bool numSign = false;
        bool denSign = false;

        pair<unsigned long long, unsigned long long> num(static_cast<unsigned long long>(first), second);
        pair<unsigned long long, unsigned long long> den(static_cast<unsigned long long>(rhs.first), rhs.second);
        if (first < 0) {
            numSign = true;
            if (!second) {
                num.first = -num.first;
                num.second = 0ull;
            } else {
                num.first = ~num.first;
                num.second = -second;
            }
        }
        if (rhs.first < 0) {
            denSign = true;
            if (!rhs.second) {
                den.first = -den.first;
                den.second = 0ull;
            } else {
                den.first = ~den.first;
                den.second = -den.second;
            }
        }

        auto r = udivmod(num, den);

        pair<Int128, Int128> res(r.first, r.second);
        if (numSign != denSign) {
            if (!r.first.second) {
                res.first.first = -res.first.first;
                res.first.second = 0ull;
            } else {
                res.first.first = ~res.first.first;
                res.first.second = -res.first.second;
            }
        }
        if (numSign) {
            if (!r.second.second) {
                res.second.first = -res.second.first;
                res.second.second = 0ull;
            } else {
                res.second.first = ~res.second.first;
                res.second.second = -res.second.second;
            }
        }

        return res;
    }

    Int128 operator /(const Int128& rhs) const {
        bool numSign = false;
        bool denSign = false;

        pair<unsigned long long, unsigned long long> num(static_cast<unsigned long long>(first), second);
        pair<unsigned long long, unsigned long long> den(static_cast<unsigned long long>(rhs.first), rhs.second);
        if (first < 0) {
            numSign = true;
            if (!second) {
                num.first = -num.first;
                num.second = 0ull;
            } else {
                num.first = ~num.first;
                num.second = -second;
            }
        }
        if (rhs.first < 0) {
            denSign = true;
            if (!rhs.second) {
                den.first = -den.first;
                den.second = 0ull;
            } else {
                den.first = ~den.first;
                den.second = -den.second;
            }
        }

        auto r = udivmod(num, den);

        Int128 res(r.first);
        if (numSign != denSign) {
            if (!res.second) {
                res.first = -res.first;
                res.second = 0ull;
            } else {
                res.first = ~res.first;
                res.second = -res.second;
            }
        }

        return res;
    }

    Int128 operator /=(const Int128& rhs) {
        auto t = operator /(rhs);
        return *this = t;
    }

    Int128 operator %(const Int128& rhs) const {
        bool numSign = false;

        pair<unsigned long long, unsigned long long> num(static_cast<unsigned long long>(first), second);
        pair<unsigned long long, unsigned long long> den(static_cast<unsigned long long>(rhs.first), rhs.second);
        if (first < 0) {
            numSign = true;
            if (!second) {
                num.first = -num.first;
                num.second = 0ull;
            } else {
                num.first = ~num.first;
                num.second = -second;
            }
        }
        if (rhs.first < 0) {
            if (!rhs.second) {
                den.first = -den.first;
                den.second = 0ull;
            } else {
                den.first = ~den.first;
                den.second = -den.second;
            }
        }

        auto r = udivmod(num, den);

        Int128 res(r.second);
        if (numSign) {
            if (!r.second.second) {
                res.first = -res.first;
                res.second = 0ull;
            } else {
                res.first = ~res.first;
                res.second = -res.second;
            }
        }

        return res;
    }

    Int128 operator %=(const Int128& rhs) {
        auto t = operator %(rhs);
        return *this = t;
    }


    Int128& operator --() {
        first -= _subborrow_u64(0, second, 1, &second);
        return *this;
    }

    Int128 operator --(int) const {
        Int128 res;
        res.first = first - _subborrow_u64(0, second, 1, &res.second);
        return res;
    }

    Int128& operator ++() {
        first += _addcarry_u64(0, second, 1, &second);
        return *this;
    }

    Int128 operator ++(int) const {
        Int128 res;
        res.first = first + _addcarry_u64(0, second, 1, &res.second);
        return res;
    }


    //--- bit operations

    Int128 operator <<(int n) const {
        if (n >= 128)
            return Int128(0ll, 0ull);
        else if (n >= 64)
            return Int128(static_cast<long long>(second << (n - 64)), 0ull);
        else
            return Int128(static_cast<long long>((first << n) | (second >> (64 - n))), second << n);
    }

    Int128& operator <<=(int n) {
        auto t = operator <<(n);
        return *this = t;
    }

    Int128 operator >>(int n) const {
        if (n >= 128)
            return Int128(-1ll, static_cast<unsigned long long>(-1ll));
        else if (n >= 64)
            return Int128(-1ll, static_cast<unsigned long long>(first >> (n - 64)));
        else
            return Int128(first >> n, (first << (64 - n)) | (second >> n));
    }

    Int128& operator >>=(int n) {
        auto t = operator >>(n);
        return *this = t;
    }

    Int128 operator |(const Int128& rhs) const {
        return Int128(first | rhs.first, second | rhs.second);
    }

    Int128& operator |=(const Int128& rhs) {
        first |= rhs.first;
        second |= rhs.second;
        return *this;
    }

    Int128 operator &(const Int128& rhs) const {
        return Int128(first & rhs.first, second & rhs.second);
    }

    Int128& operator &=(const Int128& rhs) {
        first &= rhs.first;
        second &= rhs.second;
        return *this;
    }

    Int128 operator ^(const Int128& rhs) const {
        return Int128(first ^ rhs.first, second ^ rhs.second);
    }

    Int128& operator ^=(const Int128& rhs) {
        first ^= rhs.first;
        second ^= rhs.second;
        return *this;
    }

    Int128 operator ~() const {
        return Int128(~first, ~second);
    }


    int clz() const {
        if (second)
            return ::clz(second);
        else
            return 64 + ::clz(static_cast<unsigned long long>(first));
    }

    int ctz() const {
        if (first)
            return ::ctz(static_cast<unsigned long long>(first));
        else
            return 64 + ::ctz(second);
    }

    int popcount() const {
        return ::popcount(static_cast<unsigned long long>(first)) + ::popcount(second);
    }
};

// (hi, lo)
struct UInt128 : pair<unsigned long long, unsigned long long> {
    UInt128() : pair<unsigned long long, unsigned long long>() {
    }

    UInt128(long long hi, unsigned long long lo) : pair<unsigned long long, unsigned long long>(static_cast<unsigned long long>(hi), lo) {
    }

    UInt128(unsigned long long hi, unsigned long long lo) : pair<unsigned long long, unsigned long long>(hi, lo) {
    }

    template <typename T>
    UInt128(T rhs) {
        operator =(rhs);
    }

    explicit UInt128(const pair<long long, unsigned long long>& rhs) {
        operator =(rhs);
    }

    explicit UInt128(const pair<unsigned long long, unsigned long long>& rhs) {
        operator =(rhs);
    }

    UInt128(const UInt128& rhs) {
        operator =(rhs);
    }


    UInt128& operator =(const UInt128& rhs) {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }

    UInt128& operator =(long long rhs) {
        first = 0;
        second = static_cast<unsigned long long>(rhs);
        return *this;
    }

    UInt128& operator =(unsigned long long rhs) {
        first = 0;
        second = rhs;
        return *this;
    }

    template <typename T>
    UInt128& operator =(T rhs) {
        return operator =(static_cast<long long>(rhs));
    }

    UInt128& operator =(const pair<long long, unsigned long long>& rhs) {
        first = static_cast<unsigned long long>(rhs.first);
        second = rhs.second;
        return *this;
    }

    UInt128& operator =(const pair<unsigned long long, unsigned long long>& rhs) {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }


    operator bool() const {
        return first != 0 || second != 0;
    }

    template <typename T>
    operator T() const {
        return static_cast<T>(second);
    }


    UInt128 operator -() const {
        return UInt128(!second ? -first : ~first, !second ? 0ull : -second);
    }

    const UInt128& operator +() const {
        return *this;
    }


    UInt128 operator +(const UInt128& rhs) const {
        UInt128 res;
        res.first = first + rhs.first + _addcarry_u64(0, second, rhs.second, &res.second);
        return res;
    }

    UInt128& operator +=(const UInt128& rhs) {
        first += rhs.first + _addcarry_u64(0, second, rhs.second, &second);
        return *this;
    }

    UInt128 operator -(const UInt128& rhs) const {
        UInt128 res;
        res.first = first - (rhs.first + _subborrow_u64(0, second, rhs.second, &res.second));
        return res;
    }

    UInt128& operator -=(const UInt128& rhs) {
        first -= rhs.first + _subborrow_u64(0, second, rhs.second, &second);
        return *this;
    }

    UInt128 operator *(const UInt128& rhs) const {
        UInt128 res;
        unsigned long long t;
//#ifndef __GNUC__
//        res.second = static_cast<unsigned long long>(_umul128(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
//        res.first += _umul128(second, rhs.first, &t);
//        res.first += _umul128(first, rhs.second, &t);
//#else    
        res.second = static_cast<unsigned long long>(_mulx_u64(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
        res.first += _mulx_u64(second, rhs.first, &t);
        res.first += _mulx_u64(first, rhs.second, &t);
//#endif    
        return res;
    }

    UInt128& operator *=(const UInt128& rhs) {
        auto t = operator *(rhs);
        return *this = t;
    }

    //---

    // (quotient, remainder)
    pair<UInt128, UInt128> divmod(const UInt128& rhs) const {
        auto t = ::udivmod(*this, rhs);
        return make_pair(UInt128(t.first), UInt128(t.second));
    }

    UInt128 operator /(const UInt128& rhs) const {
        auto res = ::udivmod(*this, rhs);
        return UInt128(res.first);
    }

    UInt128 operator /=(const UInt128& rhs) {
        auto res = ::udivmod(*this, rhs);
        return *this = res.first;
    }

    UInt128 operator %(const UInt128& rhs) const {
        auto res = ::udivmod(*this, rhs);
        return UInt128(res.second);
    }

    UInt128 operator %=(const UInt128& rhs) {
        auto res = ::udivmod(*this, rhs);
        return *this = res.second;
    }


    UInt128& operator --() {
        first -= _subborrow_u64(0, second, 1, &second);
        return *this;
    }

    UInt128 operator --(int) const {
        UInt128 res;
        res.first = first - _subborrow_u64(0, second, 1, &res.second);
        return res;
    }

    UInt128& operator ++() {
        first += _addcarry_u64(0, second, 1, &second);
        return *this;
    }

    UInt128 operator ++(int) const {
        UInt128 res;
        res.first = first + _addcarry_u64(0, second, 1, &res.second);
        return res;
    }

    //--- bit operations

    UInt128 operator <<(int n) const {
        if (n >= 128)
            return UInt128(0ull, 0ull);
        else if (n >= 64)
            return UInt128(second << (n - 64), 0ull);
        else
            return UInt128((first << n) | (second >> (64 - n)), second << n);
    }

    UInt128& operator <<=(int n) {
        auto t = operator <<(n);
        return *this = t;
    }

    UInt128 operator >>(int n) const {
        if (n >= 128)
            return UInt128(0ull, 0ull);
        else if (n >= 64)
            return UInt128(0ull, first >> (n - 64));
        else
            return UInt128(first >> n, (first << (64 - n)) | (second >> n));
    }

    UInt128& operator >>=(int n) {
        auto t = operator >>(n);
        return *this = t;
    }

    UInt128 operator |(const UInt128& rhs) const {
        return UInt128(first | rhs.first, second | rhs.second);
    }

    UInt128& operator |=(const UInt128& rhs) {
        first |= rhs.first;
        second |= rhs.second;
        return *this;
    }

    UInt128 operator &(const UInt128& rhs) const {
        return UInt128(first & rhs.first, second & rhs.second);
    }

    UInt128& operator &=(const UInt128& rhs) {
        first &= rhs.first;
        second &= rhs.second;
        return *this;
    }

    UInt128 operator ^(const UInt128& rhs) const {
        return UInt128(first ^ rhs.first, second ^ rhs.second);
    }

    UInt128& operator ^=(const UInt128& rhs) {
        first ^= rhs.first;
        second ^= rhs.second;
        return *this;
    }

    UInt128 operator ~() const {
        return UInt128(~first, ~second);
    }


    int clz() const {
        if (second)
            return ::clz(second);
        else
            return 64 + ::clz(first);
    }

    int ctz() const {
        if (first)
            return ::ctz(first);
        else
            return 64 + ::ctz(second);
    }

    int popcount() const {
        return ::popcount(first) + ::popcount(second);
    }
};

//---

inline int clz(const Int128& x) {
    return x.clz();
}

inline int ctz(const Int128& x) {
    return x.ctz();
}

inline int popcount(const Int128& x) {
    return x.popcount();
}

inline int clz(const UInt128& x) {
    return x.clz();
}

inline int ctz(const UInt128& x) {
    return x.ctz();
}

inline int popcount(const UInt128& x) {
    return x.popcount();
}


inline Int128 makeInt128(long long hi, unsigned long long lo) {
    return Int128(hi, lo);
}

inline UInt128 makeUInt128(unsigned long long hi, unsigned long long lo) {
    return UInt128(hi, lo);
}

#endif
