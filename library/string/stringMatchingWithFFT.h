#pragma once

// https://cp-algorithms.com/algebra/fft.html

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

struct StringMatchingWithFFT {
    // return positions in text, O(NlogN)
    static vector<int> search(const string& text, const string& pattern) {
        int N = int(text.length());
        int M = int(pattern.length());

        if (N < M)
            return{};
        
        auto c = convolute(text, pattern);
        vector<int> res;

        for (int i = M - 1; i < N; i++) {
            if (int(c[i].first + 1e-6) == M)
                res.push_back(i - M + 1);
        }

        return res;
    }

    // wildcard = { '?' }
    // return positions in text, O(NlogN)
    static vector<int> searchWithWildcard(const string& text, const string& pattern) {
        int N = int(text.length());
        int M = int(pattern.length());

        if (N < M)
            return{};

        int wildcardCount = 0;
        auto c = convolute(text, pattern, wildcardCount);
        vector<int> res;

        int req = M - wildcardCount;
        for (int i = M - 1; i < N; i++) {
            if (int(c[i].first + 1e-6) == req)
                res.push_back(i - M + 1);
        }

        return res;
    }


private:
    static bool fft(vector<pair<double,double>>& data, bool inverse = false) {
        int size = int(data.size());
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
                pair<double,double> w(cos(M_PI * j / m), sin(M_PI * j / m));
                if (!inverse)
                    w.second = -w.second;

                int step = m << 1;
                for (int i = j; i < size; i += step) {
                    pair<double, double> t(data[i + m].first * w.first - data[i + m].second * w.second,
                        data[i + m].first * w.second + data[i + m].second * w.first);
                    data[i + m].first = data[i].first - t.first;
                    data[i + m].second = data[i].second - t.second;
                    data[i].first += t.first;
                    data[i].second += t.second;
                }
            }
        }

        if (inverse) {
            for (int i = 0; i < size; i++) {
                data[i].first /= size;
                data[i].second /= size;
            }
        }

        return true;
    }

    static vector<pair<double,double>> convolute(const string& text, const string& pattern) {
        int sizeL = int(text.size());
        int sizeR = int(pattern.size());
        int sizeDst = sizeL + sizeR - 1;

#define A2C(a)  { cos(2 * M_PI * (a) / 26.0), sin(2 * M_PI * (a) / 26.0) }
        static pair<double, double> complexA[26] = {
            A2C( 0), A2C( 1), A2C( 2), A2C( 3), A2C( 4), A2C( 5), A2C( 6), A2C( 7), A2C( 8), A2C( 9),
            A2C(10), A2C(11), A2C(12), A2C(13), A2C(14), A2C(15), A2C(16), A2C(17), A2C(18), A2C(19),
            A2C(20), A2C(21), A2C(22), A2C(23), A2C(24), A2C(25)
        };
#undef A2C
#define A2C(a)  { cos(2 * M_PI * (a) / 26.0), -sin(2 * M_PI * (a) / 26.0) }
        static pair<double, double> complexB[26] = {
            A2C( 0), A2C( 1), A2C( 2), A2C( 3), A2C( 4), A2C( 5), A2C( 6), A2C( 7), A2C( 8), A2C( 9),
            A2C(10), A2C(11), A2C(12), A2C(13), A2C(14), A2C(15), A2C(16), A2C(17), A2C(18), A2C(19),
            A2C(20), A2C(21), A2C(22), A2C(23), A2C(24), A2C(25)
        };
#undef A2C

        int size = 1;
        while (size < sizeDst)
            size <<= 1;
        
        vector<pair<double,double>> A(size);
        vector<pair<double,double>> B(size);
        for (int i = 0; i < sizeL; i++)
            A[i] = complexA[text[i] - 'a'];

        for (int i = 0; i < sizeR; i++)
            //B[i] = complexB[pattern[i] - 'a'];
            B[sizeR - 1 - i] = complexB[pattern[i] - 'a'];

        fft(A);
        fft(B);

        vector<pair<double,double>> C(size);
        for (int i = 0; i < size; i++) {
            //C[i] = A[i] * B[i];
            C[i].first = A[i].first * B[i].first - A[i].second * B[i].second;
            C[i].second = A[i].first * B[i].second + A[i].second * B[i].first;
        }

        fft(C, true);

        vector<pair<double,double>> res(sizeDst);
        for (int i = 0; i < sizeDst; i++)
            res[i] = C[i];
        return res;
    }

    static vector<pair<double,double>> convolute(const string& text, const string& pattern, int& wildcardCount) {
        int sizeL = int(text.size());
        int sizeR = int(pattern.size());
        int sizeDst = sizeL + sizeR - 1;

#define A2C(a)  { cos(2 * M_PI * (a) / 26.0), sin(2 * M_PI * (a) / 26.0) }
        static pair<double, double> complexA[26] = {
            A2C( 0), A2C( 1), A2C( 2), A2C( 3), A2C( 4), A2C( 5), A2C( 6), A2C( 7), A2C( 8), A2C( 9),
            A2C(10), A2C(11), A2C(12), A2C(13), A2C(14), A2C(15), A2C(16), A2C(17), A2C(18), A2C(19),
            A2C(20), A2C(21), A2C(22), A2C(23), A2C(24), A2C(25)
        };
#undef A2C
#define A2C(a)  { cos(2 * M_PI * (a) / 26.0), -sin(2 * M_PI * (a) / 26.0) }
        static pair<double, double> complexB[26] = {
            A2C( 0), A2C( 1), A2C( 2), A2C( 3), A2C( 4), A2C( 5), A2C( 6), A2C( 7), A2C( 8), A2C( 9),
            A2C(10), A2C(11), A2C(12), A2C(13), A2C(14), A2C(15), A2C(16), A2C(17), A2C(18), A2C(19),
            A2C(20), A2C(21), A2C(22), A2C(23), A2C(24), A2C(25)
        };
#undef A2C

        wildcardCount = 0;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A(size);
        vector<pair<double,double>> B(size);
        for (int i = 0; i < sizeL; i++)
            A[i] = complexA[text[i] - 'a'];

        for (int i = 0; i < sizeR; i++) {
            if (pattern[i] == '?')
                wildcardCount++;
            else
                B[sizeR - 1 - i] = complexB[pattern[i] - 'a'];
        }

        fft(A);
        fft(B);

        vector<pair<double,double>> C(size);
        for (int i = 0; i < size; i++) {
            //C[i] = A[i] * B[i];
            C[i].first = A[i].first * B[i].first - A[i].second * B[i].second;
            C[i].second = A[i].first * B[i].second + A[i].second * B[i].first;
        }

        fft(C, true);

        vector<pair<double,double>> res(sizeDst);
        for (int i = 0; i < sizeDst; i++)
            res[i] = C[i];
        return res;
    }
};
