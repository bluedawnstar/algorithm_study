#include <vector>

using namespace std;

#include "walshHadamard.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

template <typename T>
static vector<T> slowXor(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i ^ j] += A[i] * B[j];
    }

    return C;
}

template <typename T>
static vector<T> slowOr(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i | j] += A[i] * B[j];
    }

    return C;
}

template <typename T>
static vector<T> slowAnd(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i & j] += A[i] * B[j];
    }

    return C;
}

void testWalshHadamard() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast Walsh-Hadamard transform ------------------------" << endl;
    {
        int T = 100;
        int N = 1000;

#if _DEBUG
        T = 10;
        N = 100;
#endif

        while (T-- > 0) {
            vector<long long> A(N);
            vector<long long> B(N);
            for (int i = 0; i < N; i++) {
                A[i] = RandInt32::get() % 10000;
                B[i] = RandInt32::get() % 10000;
            }

            auto ansXor = FWHT<long long>::fastXor(A, B);
            auto ansOr  = FWHT<long long>::fastOr(A, B);
            auto ansAnd = FWHT<long long>::fastAnd(A, B);

            auto gtXor = slowXor(A, B);
            auto gtOr = slowOr(A, B);
            auto gtAnd = slowAnd(A, B);

            if (ansXor != gtXor)
                cout << "Mismatched XOR!" << endl;
            if (ansOr != gtOr)
                cout << "Mismatched OR!" << endl;
            if (ansAnd != gtAnd)
                cout << "Mismatched AND!" << endl;

            assert(ansXor == gtXor);
            assert(ansOr == gtOr);
            assert(ansAnd == gtAnd);
        }
    }

    cout << "OK!" << endl;
}
