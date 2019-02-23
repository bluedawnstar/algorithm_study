#pragma once

#include <cmath>
#include <complex>
#include <vector>
#include <algorithm>

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

template <typename T>
struct Complex : public pair<T, T> {
    Complex() : pair<T, T>() {
    }

    Complex(T r, T i) : pair<T, T>(r, i) {
    }

    Complex(std::complex<T> c) : pair<T, T>(c.real(), c.imag()) {
    }

    operator std::complex<T>() const {
        return complex<T>(this->first, this->second);
    }

    Complex operator -() const {
        return Complex(-this->first, -this->second);
    }

    template <typename U>
    Complex operator +(U rhs) const {
        return Complex(this->first + rhs, this->second);
    }

    template <typename U>
    Complex operator -(U rhs) const {
        return Complex(this->first - rhs, this->second);
    }

    template <typename U>
    Complex operator *(U rhs) const {
        return Complex(this->first * rhs, this->second * rhs);
    }

    template <typename U>
    Complex operator /(U rhs) const {
        return Complex(this->first / rhs, this->second / rhs);
    }

    Complex operator +(const Complex& rhs) const {
        return Complex(this->first + rhs.first, this->second + rhs.second);
    }

    Complex operator -(const Complex& rhs) const {
        return Complex(this->first - rhs.first, this->second - rhs.second);
    }

    Complex operator *(const Complex& rhs) const {
        return Complex(this->first * rhs.first - this->second * rhs.second,
            this->first * rhs.second + this->second * rhs.first);
    }

    Complex operator /(const Complex& rhs) const {
        T divisor = rhs.first * rhs.first + rhs.second * rhs.second;
        return Complex((this->first * rhs.first + this->second * rhs.second) / divisor,
                       (this->second * rhs.first - this->first * rhs.second) / divisor);
    }

    void set(T r, T i) {
        this->first = r;
        this->second = i;
    }

    double abs() const {
        return hypot(this->first, this->second);
    }

    template <typename U>
    Complex& operator +=(U rhs) {
        this->first += rhs;
        return *this;
    }

    template <typename U>
    Complex& operator -=(U rhs) {
        this->first -= rhs;
        return *this;
    }

    template <typename U>
    Complex& operator *=(U rhs) {
        this->first *= rhs; this->second *= rhs;
        return *this;
    }

    template <typename U>
    Complex& operator /=(U rhs) {
        this->first /= rhs; this->second /= rhs;
        return *this;
    }

    Complex& operator +=(const Complex& rhs) {
        this->first += rhs.first;
        this->second += rhs.second;
        return *this;
    }

    Complex& operator -=(const Complex& rhs) {
        this->first -= rhs.first;
        this->second -= rhs.second;
        return *this;
    }

    Complex& operator *=(const Complex& rhs) {
        T t = this->first * rhs.first - this->second * rhs.second;
        this->second = this->first * rhs.second + this->second * rhs.first;
        this->first = t;
        return *this;
    }

    Complex& operator /=(const Complex& rhs) {
        T divisor = rhs.first * rhs.first + rhs.second * rhs.second;
        T t = (this->first * rhs.first + this->second * rhs.second) / divisor;
        this->second = (this->second * rhs.first - this->first * rhs.second) / divisor;
        this->first = t;
        return *this;
    }
};
