#include <list>
#include <unordered_map>

using namespace std;

#include "LRUCache.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testLRUCache() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- LRU Cache ------------------------" << endl;
    {
        LRUCache<int, int> lru(2, -1);

        lru.put(1, 10);
        lru.put(2, 20);
        lru.put(3, 30);

        int a = lru.get(1);
        if (a != -1)
            cout << "ERROR: " << a << ", " << -1 << endl;
        assert(a == -1);

        int b = lru.get(2);
        if (b == 20)
            cout << "ERROR: " << b << ", " << 20 << endl;
        assert(b == 20);

        lru.put(4, 40);

        int c = lru.get(3);
        if (c == -1)
            cout << "ERROR: " << c << ", " << -1 << endl;
        assert(c == -1);

        int d = lru.get(2);
        if (d == 20)
            cout << "ERROR: " << d << ", " << 20 << endl;
        assert(d == 20);

        int e = lru.get(4);
        if (e == 40)
            cout << "ERROR: " << e << ", " << 40 << endl;
        assert(e == 40);
    }

    cout << "OK!" << endl;
}
