#include <cmath>
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

    void set(T r, T i) {
        this->first = r;
        this->second = i;
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
};

template <typename T = double>
void dft(const vector<Complex<T>>& in, vector<Complex<T>>& out, bool inverse = false) {
    int size = (int)in.size();
    T wk, wn = 2 * M_PI / size;

    if (inverse)
        wn = -wn;

    for (int k = 0; k < size; k++) {
        out[k].set(0, 0);

        wk = k * wn;
        for (int n = 0; n < size; n++) {
            double c = cos(n * wk), s = sin(n * wk);
            out[k].first += in[n].first *  c + in[n].second * s;
            out[k].second += in[n].first * -s + in[n].second * c;
        }

        if (inverse)
            out[k] /= size;
    }
}

template <typename T = double>
bool fft(const vector<Complex<T>>& in, vector<Complex<T>>& out, bool inverse = false) {
    int size = (int)in.size();
    if ((size & (size - 1)) != 0)
        return false;

    for (int i = 0, j = 0; i < size; i++) {
        out[j] = in[i];

        int mask = size;
        while (j & (mask >>= 1))
            j &= ~mask;
        j |= mask;
    }

    for (int m = 1; m < size; m <<= 1) {
        for (int j = 0; j < m; j++) {
            Complex<T> w(cos(M_PI * j / m), sin(M_PI * j / m));
            if (!inverse)
                w.second = -w.second;

            int step = m << 1;
            for (int i = j; i < size; i += step) {
                Complex<T> t = out[i + m] * w;
                out[i + m] = out[i] - t;
                out[i] += t;
            }
        }
    }

    if (inverse) {
        for (int i = 0; i < size; i++)
            out[i] /= size;
    }

    return true;
}

template <typename T = double>
bool fft(vector<Complex<T>>& data, bool inverse = false) {
    int size = (int)data.size();
    if ((size & (size - 1)) != 0)
        return false;

    for (int i = 0, j = 0; i < size; i++) {
        if (j > i)
            swap(data[i], data[j]);

        int mask = size;
        while (j & (mask >>= 1))
            j &= ~mask;
        j |= mask;
    }

    for (int m = 1; m < size; m <<= 1) {
        for (int j = 0; j < m; j++) {
            Complex<T> w(cos(M_PI * j / m), sin(M_PI * j / m));
            if (!inverse)
                w.second = -w.second;

            int step = m << 1;
            for (int i = j; i < size; i += step) {
                Complex<T> t = data[i + m] * w;
                data[i + m] = data[i] - t;
                data[i] += t;
            }
        }
    }

    if (inverse) {
        for (int i = 0; i < size; i++)
            data[i] /= size;
    }

    return true;
}
