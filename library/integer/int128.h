#pragma once

#ifdef __GNUC__
#include <immintrin.h>

typedef __int128_t      Int128;
typedef __uint128_t     UInt128;

inline Int128 makeInt128(long long hi, unsigned long long lo) {
    return (__int128_t(hi) << 64) | lo;
}

inline UInt128 makeUInt128(unsigned long long hi, unsigned long long lo) {
    return (__uint128_t(hi) << 64) | lo;
}

#else

#ifndef __GNUC__
#include <intrin.h>
#include <immintrin.h>
#pragma warning(disable: 4146)
#else
#include <x86intrin.h>
#include <immintrin.h>
#endif

inline int clz(unsigned long long x) {
#if defined(_M_X64)
    return int(_lzcnt_u64(x));
#elif defined(__GNUC__)
    return __builtin_clzll(x);
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
#elif defined(__GNUC__)
    return __builtin_ctzll(x);
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
#elif defined(__GNUC__)
    return __builtin_popcountll(x);
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
static pair<pair<unsigned long long, unsigned long long>,  // quotient
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
                        Int128();
                        Int128(long long hi, unsigned long long lo);
                        Int128(unsigned long long hi, unsigned long long lo);
                        Int128(char rhs);
                        Int128(unsigned char rhs);
                        Int128(short rhs);
                        Int128(unsigned short rhs);
                        Int128(int rhs);
                        Int128(unsigned int rhs);
                        Int128(long rhs);
                        Int128(unsigned long rhs);
                        Int128(long long rhs);
                        Int128(unsigned long long rhs);
    explicit            Int128(const pair<long long, unsigned long long>& rhs);
    explicit            Int128(const pair<unsigned long long, unsigned long long>& rhs);
                        Int128(const Int128& rhs);

            Int128&     operator =(char rhs);
            Int128&     operator =(unsigned char rhs);
            Int128&     operator =(short rhs);
            Int128&     operator =(unsigned short rhs);
            Int128&     operator =(int rhs);
            Int128&     operator =(unsigned int rhs);
            Int128&     operator =(long rhs);
            Int128&     operator =(unsigned long rhs);
            Int128&     operator =(long long rhs);
            Int128&     operator =(unsigned long long rhs);
            Int128&     operator =(const pair<long long, unsigned long long>& rhs);
            Int128&     operator =(const pair<unsigned long long, unsigned long long>& rhs);
            Int128&     operator =(const Int128& rhs);

                        operator bool() const;
                        operator char() const;
                        operator unsigned char() const;
                        operator short() const;
                        operator unsigned short() const;
                        operator int() const;
                        operator unsigned int() const;
                        operator long() const;
                        operator unsigned long() const;
                        operator long long() const;
                        operator unsigned long long() const;

            Int128        operator -() const;
            const Int128& operator +() const;

            Int128      operator + (const Int128& rhs) const;
            Int128&     operator +=(const Int128& rhs);

            Int128      operator - (const Int128& rhs) const;
            Int128&     operator -=(const Int128& rhs);

            Int128      operator * (const Int128& rhs) const;
            Int128&     operator *=(const Int128& rhs);

            // (quotient, remainder)
            pair<Int128, Int128> divmod(Int128 rhs) const;

            Int128      operator / (Int128 rhs) const;
            Int128      operator /=(Int128 rhs);

            Int128      operator % (Int128 rhs) const;
            Int128      operator %=(Int128 rhs);

            Int128&     operator --();
            Int128      operator --(int) const;

            Int128&     operator ++();
            Int128      operator ++(int) const;

    //--- bit operations

            Int128      operator << (int n) const;
            Int128&     operator <<=(int n);
            Int128      operator >> (int n) const;
            Int128&     operator >>=(int n);

            Int128      operator | (const Int128& rhs) const;
            Int128&     operator |=(const Int128& rhs);
            Int128      operator & (const Int128& rhs) const;
            Int128&     operator &=(const Int128& rhs);
            Int128      operator ^ (const Int128& rhs) const;
            Int128&     operator ^=(const Int128& rhs);

            Int128      operator ~() const;

            int         clz() const;
            int         ctz() const;
            int         popcount() const;
};

// (hi, lo)
struct UInt128 : pair<unsigned long long, unsigned long long> {
                        UInt128();
                        UInt128(long long hi, unsigned long long lo);
                        UInt128(unsigned long long hi, unsigned long long lo);
                        UInt128(char rhs);
                        UInt128(unsigned char rhs);
                        UInt128(short rhs);
                        UInt128(unsigned short rhs);
                        UInt128(int rhs);
                        UInt128(unsigned int rhs);
                        UInt128(long rhs);
                        UInt128(unsigned long rhs);
                        UInt128(long long rhs);
                        UInt128(unsigned long long rhs);
    explicit            UInt128(const pair<long long, unsigned long long>& rhs);
    explicit            UInt128(const pair<unsigned long long, unsigned long long>& rhs);
                        UInt128(const UInt128& rhs);

            UInt128&    operator =(char rhs);
            UInt128&    operator =(unsigned char rhs);
            UInt128&    operator =(short rhs);
            UInt128&    operator =(unsigned short rhs);
            UInt128&    operator =(int rhs);
            UInt128&    operator =(unsigned int rhs);
            UInt128&    operator =(long rhs);
            UInt128&    operator =(unsigned long rhs);
            UInt128&    operator =(long long rhs);
            UInt128&    operator =(unsigned long long rhs);
            UInt128&    operator =(const pair<long long, unsigned long long>& rhs);
            UInt128&    operator =(const pair<unsigned long long, unsigned long long>& rhs);
            UInt128&    operator =(const UInt128& rhs);

                        operator bool() const;
                        operator char() const;
                        operator unsigned char() const;
                        operator short() const;
                        operator unsigned short() const;
                        operator int() const;
                        operator unsigned int() const;
                        operator long() const;
                        operator unsigned long() const;
                        operator long long() const;
                        operator unsigned long long() const;

            UInt128        operator -() const;
            const UInt128& operator +() const;

            UInt128     operator + (const UInt128& rhs) const;
            UInt128&    operator +=(const UInt128& rhs);

            UInt128     operator - (const UInt128& rhs) const;
            UInt128&    operator -=(const UInt128& rhs);

            UInt128     operator * (const UInt128& rhs) const;
            UInt128&    operator *=(const UInt128& rhs);

            // (quotient, remainder)
            pair<UInt128, UInt128> divmod(const UInt128& rhs) const;

            UInt128     operator / (UInt128 rhs) const;
            UInt128     operator /=(UInt128 rhs);

            UInt128     operator % (UInt128 rhs) const;
            UInt128     operator %=(UInt128 rhs);

            UInt128&    operator --();
            UInt128     operator --(int) const;

            UInt128&    operator ++();
            UInt128     operator ++(int) const;

    //--- bit operations

            UInt128     operator << (int n) const;
            UInt128&    operator <<=(int n);
            UInt128     operator >> (int n) const;
            UInt128&    operator >>=(int n);

            UInt128     operator | (const UInt128& rhs) const;
            UInt128&    operator |=(const UInt128& rhs);
            UInt128     operator & (const UInt128& rhs) const;
            UInt128&    operator &=(const UInt128& rhs);
            UInt128     operator ^ (const UInt128& rhs) const;
            UInt128&    operator ^=(const UInt128& rhs);

            UInt128     operator ~() const;

            int         clz() const;
            int         ctz() const;
            int         popcount() const;
};

//--------- Int128 ------------------------------------------------------------

inline Int128::Int128()
    : pair<long long, unsigned long long>() {
}

inline Int128::Int128(long long hi, unsigned long long lo)
    : pair<long long, unsigned long long>(hi, lo) {
}

inline Int128::Int128(unsigned long long hi, unsigned long long lo)
    : pair<long long, unsigned long long>(static_cast<long long>(hi), lo) {
}

inline Int128::Int128(char rhs) {
    operator =(rhs);
}

inline Int128::Int128(unsigned char rhs) {
    operator =(rhs);
}

inline Int128::Int128(short rhs) {
    operator =(rhs);
}

inline Int128::Int128(unsigned short rhs) {
    operator =(rhs);
}

inline Int128::Int128(int rhs) {
    operator =(rhs);
}

inline Int128::Int128(unsigned int rhs) {
    operator =(rhs);
}

inline Int128::Int128(long rhs) {
    operator =(rhs);
}

inline Int128::Int128(unsigned long rhs) {
    operator =(rhs);
}

inline Int128::Int128(long long rhs) {
    operator =(rhs);
}

inline Int128::Int128(unsigned long long rhs) {
    operator =(rhs);
}

inline Int128::Int128(const pair<long long, unsigned long long>& rhs) {
    operator =(rhs);
}

inline Int128::Int128(const pair<unsigned long long, unsigned long long>& rhs) {
    operator =(rhs);
}

inline Int128::Int128(const Int128& rhs) {
    operator =(rhs);
}

inline Int128& Int128::operator =(const Int128& rhs) {
    first = rhs.first;
    second = rhs.second;
    return *this;
}

inline Int128& Int128::operator =(long long rhs) {
    if (rhs < 0) {
        first = -1ll;
        second = static_cast<unsigned long long>(rhs);
    } else {
        first = 0;
        second = static_cast<unsigned long long>(rhs);
    }
    return *this;
}

inline Int128& Int128::operator =(unsigned long long rhs) {
    first = 0;
    second = rhs;
    return *this;
}

inline Int128& Int128::operator =(char rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline Int128& Int128::operator =(unsigned char rhs) {
    return operator =(static_cast<unsigned long long>(rhs));
}

inline Int128& Int128::operator =(short rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline Int128& Int128::operator =(unsigned short rhs) {
    return operator =(static_cast<unsigned long long>(rhs));
}

inline Int128& Int128::operator =(int rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline Int128& Int128::operator =(unsigned int rhs) {
    return operator =(static_cast<unsigned long long>(rhs));
}

inline Int128& Int128::operator =(long rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline Int128& Int128::operator =(unsigned long rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline Int128& Int128::operator =(const pair<long long, unsigned long long>& rhs) {
    first = rhs.first;
    second = rhs.second;
    return *this;
}

inline Int128& Int128::operator =(const pair<unsigned long long, unsigned long long>& rhs) {
    first = static_cast<long long>(rhs.first);
    second = rhs.second;
    return *this;
}

inline Int128::operator bool() const {
    return first != 0 || second != 0;
}

inline Int128::operator char() const {
    return static_cast<char>(second);
}

inline Int128::operator unsigned char() const {
    return static_cast<unsigned char>(second);
}

inline Int128::operator short() const {
    return static_cast<short>(second);
}

inline Int128::operator unsigned short() const {
    return static_cast<unsigned short>(second);
}

inline Int128::operator int() const {
    return static_cast<int>(second);
}

inline Int128::operator unsigned int() const {
    return static_cast<unsigned int>(second);
}

inline Int128::operator long() const {
    return static_cast<long>(second);
}

inline Int128::operator unsigned long() const {
    return static_cast<unsigned long>(second);
}

inline Int128::operator long long() const {
    return static_cast<long long>(second);
}

inline Int128::operator unsigned long long() const {
    return static_cast<unsigned long long>(second);
}

inline Int128 Int128::operator -() const {
    return Int128(!second ? -first : ~first, !second ? 0ull : -second);
}

inline const Int128& Int128::operator +() const {
    return *this;
}

inline Int128 Int128::operator +(const Int128& rhs) const {
    Int128 res;
    res.first = first + rhs.first + _addcarry_u64(0, second, rhs.second, &res.second);
    return res;
}

inline Int128& Int128::operator +=(const Int128& rhs) {
    first += rhs.first + _addcarry_u64(0, second, rhs.second, &second);
    return *this;
}

inline Int128 Int128::operator -(const Int128& rhs) const {
    Int128 res;
    res.first = first - (rhs.first + _subborrow_u64(0, second, rhs.second, &res.second));
    return res;
}

inline Int128& Int128::operator -=(const Int128& rhs) {
    first -= rhs.first + _subborrow_u64(0, second, rhs.second, &second);
    return *this;
}

inline Int128 Int128::operator *(const Int128& rhs) const {
    Int128 res;
//#ifndef __GNUC__
//    long long t;
//    res.second = static_cast<unsigned long long>(_umul128(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
//    res.first += _mul128(second, rhs.first, &t);
//    res.first += _mul128(first, rhs.second, &t);
//#else    
    unsigned long long t;
    res.second = static_cast<unsigned long long>(_mulx_u64(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
    res.first += _mulx_u64(second, static_cast<unsigned long long>(rhs.first), &t);
    res.first += _mulx_u64(static_cast<unsigned long long>(first), rhs.second, &t);
//#endif
    return res;
}

inline Int128& Int128::operator *=(const Int128& rhs) {
    auto t = operator *(rhs);
    return *this = t;
}

//---

// (quotient, remainder)
inline pair<Int128,Int128> Int128::divmod(Int128 rhs) const {
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

inline Int128 Int128::operator /(Int128 rhs) const {
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

inline Int128 Int128::operator /=(Int128 rhs) {
    auto t = operator /(rhs);
    return *this = t;
}

inline Int128 Int128::operator %(Int128 rhs) const {
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

inline Int128 Int128::operator %=(Int128 rhs) {
    auto t = operator %(rhs);
    return *this = t;
}


inline Int128& Int128::operator --() {
    first -= _subborrow_u64(0, second, 1, &second);
    return *this;
}

inline Int128 Int128::operator --(int) const {
    Int128 res;
    res.first = first - _subborrow_u64(0, second, 1, &res.second);
    return res;
}

inline Int128& Int128::operator ++() {
    first += _addcarry_u64(0, second, 1, &second);
    return *this;
}

inline Int128 Int128::operator ++(int) const {
    Int128 res;
    res.first = first + _addcarry_u64(0, second, 1, &res.second);
    return res;
}


//--- bit operations

inline Int128 Int128::operator <<(int n) const {
    if (n >= 128)
        return Int128(0ll, 0ull);
    else if (n >= 64)
        return Int128(static_cast<long long>(second << (n - 64)), 0ull);
    else
        return Int128(static_cast<long long>((first << n) | (second >> (64 - n))), second << n);
}

inline Int128& Int128::operator <<=(int n) {
    auto t = operator <<(n);
    return *this = t;
}

inline Int128 Int128::operator >>(int n) const {
    if (n >= 128)
        return Int128(-1ll, static_cast<unsigned long long>(-1ll));
    else if (n >= 64)
        return Int128(-1ll, static_cast<unsigned long long>(first >> (n - 64)));
    else
        return Int128(first >> n, (first << (64 - n)) | (second >> n));
}

inline Int128& Int128::operator >>=(int n) {
    auto t = operator >>(n);
    return *this = t;
}

inline Int128 Int128::operator |(const Int128& rhs) const {
    return Int128(first | rhs.first, second | rhs.second);
}

inline Int128& Int128::operator |=(const Int128& rhs) {
    first  |= rhs.first;
    second |= rhs.second;
    return *this;
}

inline Int128 Int128::operator &(const Int128& rhs) const {
    return Int128(first & rhs.first, second & rhs.second);
}

inline Int128& Int128::operator &=(const Int128& rhs) {
    first  &= rhs.first;
    second &= rhs.second;
    return *this;
}

inline Int128 Int128::operator ^(const Int128& rhs) const {
    return Int128(first ^ rhs.first, second ^ rhs.second);
}

inline Int128& Int128::operator ^=(const Int128& rhs) {
    first  ^= rhs.first;
    second ^= rhs.second;
    return *this;
}

inline Int128 Int128::operator ~() const {
    return Int128(~first, ~second);
}


inline int Int128::clz() const {
    if (second)
        return ::clz(second);
    else
        return 64 + ::clz(static_cast<unsigned long long>(first));
}

inline int Int128::ctz() const {
    if (first)
        return ::ctz(static_cast<unsigned long long>(first));
    else
        return 64 + ::ctz(second);
}

inline int Int128::popcount() const {
    return ::popcount(static_cast<unsigned long long>(first)) + ::popcount(second);
}


//--------- UInt128 ------------------------------------------------------------

inline UInt128::UInt128()
    : pair<unsigned long long, unsigned long long>() {
}

inline UInt128::UInt128(long long hi, unsigned long long lo)
    : pair<unsigned long long, unsigned long long>(static_cast<unsigned long long>(hi), lo) {
}

inline UInt128::UInt128(unsigned long long hi, unsigned long long lo)
    : pair<unsigned long long, unsigned long long>(hi, lo) {
}

inline UInt128::UInt128(char rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(unsigned char rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(short rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(unsigned short rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(int rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(unsigned int rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(long rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(unsigned long rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(long long rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(unsigned long long rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(const pair<long long, unsigned long long>& rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(const pair<unsigned long long, unsigned long long>& rhs) {
    operator =(rhs);
}

inline UInt128::UInt128(const UInt128& rhs) {
    operator =(rhs);
}

inline UInt128& UInt128::operator =(const UInt128& rhs) {
    first = rhs.first;
    second = rhs.second;
    return *this;
}

inline UInt128& UInt128::operator =(long long rhs) {
    first = 0;
    second = static_cast<unsigned long long>(rhs);
    return *this;
}

inline UInt128& UInt128::operator =(unsigned long long rhs) {
    first = 0;
    second = rhs;
    return *this;
}

inline UInt128& UInt128::operator =(char rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline UInt128& UInt128::operator =(unsigned char rhs) {
    return operator =(static_cast<unsigned long long>(rhs));
}

inline UInt128& UInt128::operator =(short rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline UInt128& UInt128::operator =(unsigned short rhs) {
    return operator =(static_cast<unsigned long long>(rhs));
}

inline UInt128& UInt128::operator =(int rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline UInt128& UInt128::operator =(unsigned int rhs) {
    return operator =(static_cast<unsigned long long>(rhs));
}

inline UInt128& UInt128::operator =(long rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline UInt128& UInt128::operator =(unsigned long rhs) {
    return operator =(static_cast<long long>(rhs));
}

inline UInt128& UInt128::operator =(const pair<long long, unsigned long long>& rhs) {
    first = static_cast<unsigned long long>(rhs.first);
    second = rhs.second;
    return *this;
}

inline UInt128& UInt128::operator =(const pair<unsigned long long, unsigned long long>& rhs) {
    first = rhs.first;
    second = rhs.second;
    return *this;
}

inline UInt128::operator bool() const {
    return first != 0 || second != 0;
}

inline UInt128::operator char() const {
    return static_cast<char>(second);
}

inline UInt128::operator unsigned char() const {
    return static_cast<unsigned char>(second);
}

inline UInt128::operator short() const {
    return static_cast<short>(second);
}

inline UInt128::operator unsigned short() const {
    return static_cast<unsigned short>(second);
}

inline UInt128::operator int() const {
    return static_cast<int>(second);
}

inline UInt128::operator unsigned int() const {
    return static_cast<unsigned int>(second);
}

inline UInt128::operator long() const {
    return static_cast<long>(second);
}

inline UInt128::operator unsigned long() const {
    return static_cast<unsigned long>(second);
}

inline UInt128::operator long long() const {
    return static_cast<long long>(second);
}

inline UInt128::operator unsigned long long() const {
    return static_cast<unsigned long long>(second);
}

inline UInt128 UInt128::operator -() const {
    return UInt128(!second ? -first : ~first, !second ? 0ull : -second);
}

inline const UInt128& UInt128::operator +() const {
    return *this;
}

inline UInt128 UInt128::operator +(const UInt128& rhs) const {
    UInt128 res;
    res.first = first + rhs.first + _addcarry_u64(0, second, rhs.second, &res.second);
    return res;
}

inline UInt128& UInt128::operator +=(const UInt128& rhs) {
    first += rhs.first + _addcarry_u64(0, second, rhs.second, &second);
    return *this;
}

inline UInt128 UInt128::operator -(const UInt128& rhs) const {
    UInt128 res;
    res.first = first - (rhs.first + _subborrow_u64(0, second, rhs.second, &res.second));
    return res;
}

inline UInt128& UInt128::operator -=(const UInt128& rhs) {
    first -= rhs.first + _subborrow_u64(0, second, rhs.second, &second);
    return *this;
}

inline UInt128 UInt128::operator *(const UInt128& rhs) const {
    UInt128 res;
    unsigned long long t;
//#ifndef __GNUC__
//    res.second = static_cast<unsigned long long>(_umul128(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
//    res.first += _umul128(second, rhs.first, &t);
//    res.first += _umul128(first, rhs.second, &t);
//#else    
    res.second = static_cast<unsigned long long>(_mulx_u64(second, rhs.second, reinterpret_cast<unsigned long long*>(&res.first)));
    res.first += _mulx_u64(second, rhs.first, &t);
    res.first += _mulx_u64(first, rhs.second, &t);
//#endif    
    return res;
}

inline UInt128& UInt128::operator *=(const UInt128& rhs) {
    auto t = operator *(rhs);
    return *this = t;
}

//---

// (quotient, remainder)
inline pair<UInt128, UInt128> UInt128::divmod(const UInt128& rhs) const {
    auto t = ::udivmod(*this, rhs);
    return make_pair(UInt128(t.first), UInt128(t.second));
}

inline UInt128 UInt128::operator /(UInt128 rhs) const {
    auto res = ::udivmod(*this, rhs);
    return UInt128(res.first);
}

inline UInt128 UInt128::operator /=(UInt128 rhs) {
    auto res = ::udivmod(*this, rhs);
    return *this = res.first;
}

inline UInt128 UInt128::operator %(UInt128 rhs) const {
    auto res = ::udivmod(*this, rhs);
    return UInt128(res.second);
}

inline UInt128 UInt128::operator %=(UInt128 rhs) {
    auto res = ::udivmod(*this, rhs);
    return *this = res.second;
}


inline UInt128& UInt128::operator --() {
    first -= _subborrow_u64(0, second, 1, &second);
    return *this;
}

inline UInt128 UInt128::operator --(int) const {
    UInt128 res;
    res.first = first - _subborrow_u64(0, second, 1, &res.second);
    return res;
}

inline UInt128& UInt128::operator ++() {
    first += _addcarry_u64(0, second, 1, &second);
    return *this;
}

inline UInt128 UInt128::operator ++(int) const {
    UInt128 res;
    res.first = first + _addcarry_u64(0, second, 1, &res.second);
    return res;
}

//--- bit operations

inline UInt128 UInt128::operator <<(int n) const {
    if (n >= 128)
        return UInt128(0ull, 0ull);
    else if (n >= 64)
        return UInt128(second << (n - 64), 0ull);
    else
        return UInt128((first << n) | (second >> (64 - n)), second << n);
}

inline UInt128& UInt128::operator <<=(int n) {
    auto t = operator <<(n);
    return *this = t;
}

inline UInt128 UInt128::operator >>(int n) const {
    if (n >= 128)
        return UInt128(0ull, 0ull);
    else if (n >= 64)
        return UInt128(0ull, first >> (n - 64));
    else
        return UInt128(first >> n, (first << (64 - n)) | (second >> n));
}

inline UInt128& UInt128::operator >>=(int n) {
    auto t = operator >>(n);
    return *this = t;
}

inline UInt128 UInt128::operator |(const UInt128& rhs) const {
    return UInt128(first | rhs.first, second | rhs.second);
}

inline UInt128& UInt128::operator |=(const UInt128& rhs) {
    first |= rhs.first;
    second |= rhs.second;
    return *this;
}

inline UInt128 UInt128::operator &(const UInt128& rhs) const {
    return UInt128(first & rhs.first, second & rhs.second);
}

inline UInt128& UInt128::operator &=(const UInt128& rhs) {
    first &= rhs.first;
    second &= rhs.second;
    return *this;
}

inline UInt128 UInt128::operator ^(const UInt128& rhs) const {
    return UInt128(first ^ rhs.first, second ^ rhs.second);
}

inline UInt128& UInt128::operator ^=(const UInt128& rhs) {
    first ^= rhs.first;
    second ^= rhs.second;
    return *this;
}

inline UInt128 UInt128::operator ~() const {
    return UInt128(~first, ~second);
}


inline int UInt128::clz() const {
    if (second)
        return ::clz(second);
    else
        return 64 + ::clz(first);
}

inline int UInt128::ctz() const {
    if (first)
        return ::ctz(first);
    else
        return 64 + ::ctz(second);
}

inline int UInt128::popcount() const {
    return ::popcount(first) + ::popcount(second);
}


//---

inline Int128 makeInt128(long long hi, unsigned long long lo) {
    return Int128(hi, lo);
}

inline UInt128 makeUInt128(unsigned long long hi, unsigned long long lo) {
    return UInt128(hi, lo);
}
#endif
