#pragma once

// recursive
template <typename T>
T gcd(T p, T q);

// iterative
template <typename T>
T gcd2(T p, T q);

template <typename T>
T lcm(T p, T q);

// a * x + b * y = gcd(a, b)
template <typename T>
T extGcd(T a, T b, T& x, T& y);
