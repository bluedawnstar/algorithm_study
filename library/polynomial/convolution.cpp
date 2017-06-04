using namespace std;

#include "fft.h"

#if 0
// not implemented!!!
template <typename T = int>
vector<T> convolution(const vector<T>& left, const vector<T>& right) {
    vector<T> res(left.size() + right.size() - 1);

    for (int i = 0; i < (int)right.size(); i++) {
        for (int j = 0; j < (int)left.size(); j++) {
            res[i + j] += left[j] * right[i];
        }
    }

    return res;
}

// not implemented!!!
template <typename T = int>
vector<T> convolutionFFT(const vector<T>& left, const vector<T>& right) {
    int sizeL = (int)left.size();
    int sizeR = (int)right.size();
    int sizeDst = sizeL + sizeR - 1;

    int size = 1;
    while (size < sizeDst)
        size <<= 1;

    vector<Complex<double>> A(size);
    vector<Complex<double>> B(size);
    for (int i = 0; i < sizeL; i++)
        A[i].first = left[i];
    for (int i = 0; i < sizeR; i++)
        B[i].first = right[i];

    fft(A);
    fft(B);

    vector<Complex<double>> C(size);
    for (int i = 0; i < size; i++)
        C[i] = A[i] * B[i];

    fft(C, true);

    vector<T> res(sizeDst);
    for (int i = 0; i < sizeDst; i++)
        res[i] = T(C[i].first + 0.5);

    return res;
}
#endif

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testConvolution() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Convolution ------------------------------" << endl;

    //...
    //vector<int> outMult1 = multPoly(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    //vector<int> outMult2 = multPolyFFT(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    //assert(outMult1 == outMult2);

    cout << "OK!" << endl;
}
