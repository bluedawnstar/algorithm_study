#pragma once

#include "fractionAlgo.h"

//----------- Fraction class --------------------------------

template <typename T>
struct Fraction {
    T num;
    T denom;

    explicit Fraction(T x) : num(x), denom(1) {
    }

    Fraction(T _num, T _denom) : num(_num), denom(_denom) {
    }

    bool isNaN() const {
        return denom == 0;
    }

    bool isZero() const {
        return num == 0;
    }

    T gcd() const {
        return gcd(num, denom);
    }

    T floor() const {
        return num / denom;
    }

    T ceil() const {
        return (num + denom - 1) / denom;
    }

    void reduce() {
        T g = gcd();
        num /= g;
        denom /= g;
    }

    Fraction copyReduce() const {
        T g = gcd();
        return Fraction(num / g, denom / g);
    }

    Fraction inverse() const {
        return Fraction(denom, num);
    }

    Fraction& operator +=(const Fraction& rhs) {
        T g = gcd(denom, rhs.denom);
        num = num * rhs.denom / g + rhs.num * denom / g;
        denom = denom / g * rhs.denom;
        return *this;
    }

    Fraction& operator -=(const Fraction& rhs) {
        T g = gcd(denom, rhs.denom);
        num = num * rhs.denom / g - rhs.num * denom / g;
        denom = denom / g * rhs.denom;
        return *this;
    }

    Fraction& operator +=(T rhs) {
        num += denom * rhs;
        return *this;
    }

    Fraction& operator -=(T rhs) {
        num -= denom * rhs;
        return *this;
    }

    Fraction& operator *=(const Fraction& rhs) {
        num *= rhs.num;
        denom *= rhs.denom;
        return *this;
    }

    Fraction& operator /=(const Fraction& rhs) {
        num *= rhs.denom;
        denom *= rhs.num;
        return *this;
    }

    Fraction& operator *=(T rhs) {
        num *= rhs;
        return *this;
    }

    Fraction& operator /=(T rhs) {
        denom *= rhs;
        return *this;
    }

    Fraction operator +(const Fraction& rhs) const {
        T g = gcd(denom, rhs.denom);
        return Fraction(num * (rhs.denom / g) + rhs.num * (denom / g), denom / g * rhs.denom);
    }

    Fraction operator -(const Fraction& rhs) const {
        T g = gcd(denom, rhs.denom);
        return Fraction(num * (rhs.denom / g) - rhs.num * (denom / g), denom / g * rhs.denom);
    }

    Fraction operator +(T rhs) const {
        return Fraction(num + denom * rhs, denom);
    }

    Fraction operator -(T rhs) const {
        return Fraction(num - denom * rhs, denom);
    }


    Fraction operator *(const Fraction& rhs) const {
        return Fraction(num * rhs.num, denom * rhs.denom);
    }

    Fraction operator /(const Fraction& rhs) const {
        return Fraction(num * rhs.denom, denom * rhs.num);
    }

    Fraction operator *(T rhs) const {
        return Fraction(num * rhs, denom);
    }

    Fraction operator /(T rhs) const {
        return Fraction(num, denom * rhs);
    }

    Fraction operator -() const {
        return Fraction(-num, denom);
    }

    Fraction operator +() const {
        return Fraction(num, denom);
    }

    bool operator ==(const Fraction& rhs) const {
        return num * rhs.denom == denom * rhs.num;
    }

    bool operator !=(const Fraction& rhs) const {
        return !operator ==(rhs);
    }

    bool operator <(const Fraction& rhs) const {
        return num * rhs.denom < denom * rhs.num;
    }

    bool operator <=(const Fraction& rhs) const {
        return num * rhs.denom <= denom * rhs.num;
    }

    bool operator >(const Fraction& rhs) const {
        return num * rhs.denom > denom * rhs.num;
    }

    bool operator >=(const Fraction& rhs) const {
        return num * rhs.denom >= denom * rhs.num;
    }

    static T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }
};
