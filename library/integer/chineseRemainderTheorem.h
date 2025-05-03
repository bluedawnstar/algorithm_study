#pragma once

#include "intMod.h"

// https://librewiki.net/wiki/%EC%A4%91%EA%B5%AD%EC%9D%B8%EC%9D%98_%EB%82%98%EB%A8%B8%EC%A7%80_%EC%A0%95%EB%A6%AC 
// https://e-maxx-eng.appspot.com/algebra/chinese-remainder-theorem.html
// https://forthright48.com/chinese-remainder-theorem-part-1-coprime-moduli/
// https://forthright48.com/chinese-remainder-theorem-part-2-non-coprime-moduli/

/*
중국인의 나머지 정리는, 어떤 수 x를 서로소인 m_1, m_2, m_3, ..., m_n으로 각각 나눴을 때 나머지가 r_1, r_2, r_3, ..., r_n 이라고 할 때
x는 m_1 * m_2 * m_3 * ... * m_n 의 modular 연산 안에서 해가 유일하다는 것을 말한다.
이 x를 구하는 과정은 다음과 같다.

1) m = m_1 * m_2 * m_3 * ... * m_n이라고 할때, n_k = m / m_k라고 하자.
   n_k * s_k = 1 (mod m_k) 인 s_k를 구한다(1 <= k <= n)
   (Modular Multiplicative Inverse)
2) x = r_1 * n_1 * s_1 + r_2 * n_2 * s_2 + ... + r_k * n_k * s_k + ... + r_n * n_n * s_n (mod m)
*/

// find minimum X (vector version)
// X % m[i] = r[i], all m[i] are coprime
template <typename T>
pair<T, T> solveCRT(const vector<T>& m, const vector<T>& r, int n) {
    T mm = 1;
    for (int i = 0; i < n; i++)
        mm *= m[i];

    T res = 0;

    for (int i = 0; i < n; i++) {
        T nk = mm / m[i];
        res += r[i] * nk * modInv(nk, m[i]);
    }

    return make_pair(res % mm, mm);
}

// find minimum X (array version)
// X % m[i] = r[i], all m[i] are coprime
template <typename T>
pair<T, T> solveCRT(const T m[], const T r[], int n) {
    T mm = 1;
    for (int i = 0; i < n; i++)
        mm *= m[i];

    T res = 0;

    for (int i = 0; i < n; i++) {
        T nk = mm / m[i];
        res += r[i] * nk * modInv(nk, m[i]);
    }

    return make_pair(res % mm, mm);
}


// Chinese remainder theorem (special case):
//      find x satisfying x % m1 = r1, x % m2 = r2 (x is unique modulo M = lcm(m1, m2))
// return (x, M), if failed then (0, -1)
template <typename T>
inline pair<T, T> solveCRTExt(T m1, T r1, T m2, T r2) {
    T s, t;
    T g = extGcd(m1, m2, s, t);
    if (r1 % g != r2 % g)
        return make_pair(0, -1);
    T m = m1 * m2;
    return make_pair((((s * r2 * m1 + t * r1 * m2) % m + m) % m) / g, m / g);
}

// Chinese remainder theorem:
//      find x satisfying x % m[i] = r[i] for all i (x is unique modulo M = lcm(m[i]))
// return (x, M), if failed then (0, -1)
// 
// NOTE: it is not necessary that all m[i] are coprime
template <typename T>
inline pair<T, T> solveCRTExt(const vector<T>& m, const vector<T>& r, int n) {
    auto res = make_pair(r[0], m[0]);
    for (int i = 1; i < n; i++) {
        res = solveCRTExt(res.second, res.first, m[i], r[i]);
        if (res.second == -1)
            break;
    }
    return res;
}

// Chinese remainder theorem:
//      find x satisfying x % m[i] = r[i] for all i (x is unique modulo M = lcm(m[i]))
// return (x, M), if failed then (0, -1)
// 
// NOTE: it is not necessary that all m[i] are coprime
template <typename T>
inline pair<T, T> solveCRTExt(const T m[], const T r[], int n) {
    auto res = make_pair(r[0], m[0]);
    for (int i = 1; i < n; i++) {
        res = solveCRTExt(res.second, res.first, m[i], r[i]);
        if (res.second == -1)
            break;
    }
    return res;
}
