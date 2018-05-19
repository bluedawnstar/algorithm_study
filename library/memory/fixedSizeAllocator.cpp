#include <vector>
#include <set>
#include <algorithm>

using namespace std;

#include "fixedSizeAllocator.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testBlockAllocator() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fixed Size Allocator ----------------------------" << endl;
    {
        TypeAllocator<long long> allocator(128);

        int N = 1000;
        vector<long long*> v(N);
        for (int i = 0; i < N; i++)
            v[i] = allocator.construct(i);

        for (int i = 0; i < N; i++)
            assert(*v[i] == i);
    }
    {
        TypeAllocator<long long> allocator(128);

        int N = 1000;
        vector<long long*> v(N);
        for (int i = 0; i < N; i++) {
            v[i] = allocator.construct();
            assert(*v[i] == 0);
            *v[i] = i;
        }

        for (int i = 0; i < N; i++)
            assert(*v[i] == i);
    }
    {
        TypeAllocator<long long> allocator(128);

        //int N = 1000;
        int N = 128 * 8;
        vector<long long*> v(N);
        for (int i = 0; i < N; i++)
            v[i] = allocator.construct(i);

        for (int i = 0; i < N; i++) {
            assert(*v[i] == i);
            allocator.destroy(v[i]);
        }

        for (int i = 0; i < N; i++)
            v[i] = allocator.construct(i);

        for (int i = 0; i < N; i++) {
            assert(*v[i] == i);
            allocator.destroy(v[i]);
        }
    }

    cout << "OK!" << endl;
}
