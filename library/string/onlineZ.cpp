#include <string>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

#include "onlineZ.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testOnlineZ() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Online Z algorithm --------------------------------" << endl;
    {
                  //012345678901234567890
        string s = "abcabcdabcabcdabcabcd";
        int gt[]{
            21, // "abcabcdabcabcdabcabcd"
             0, // "bcabcdabcabcdabcabcd"
             0, // "cabcdabcabcdabcabcd"
             3, // "abcdabcabcdabcabcd"
             0, // "bcdabcabcdabcabcd"
             0, // "cdabcabcdabcabcd"
             0, // "dabcabcdabcabcd"
            14, // "abcabcdabcabcd"
             0, // "bcabcdabcabcd"
             0, // "cabcdabcabcd"
             3, // "abcdabcabcd"
             0, // "bcdabcabcd"
             0, // "cdabcabcd"
             0, // "dabcabcd"
             7, // "abcabcd"
             0, // "bcabcd"
             0, // "cabcd"
             3, // "abcd"
             0, // "bcd"
             0, // "cd"
             0  // "d"
        };

        OnlineZAlgorithm oz;

        for (int i = 0; i < int(s.length()); i++)
            oz.extend(s[i]);

        for (int i = 0; i < int(s.length()); i++)
            assert(oz.query(i) == gt[i]);
    }
    {
#ifdef _DEBUG
        const int N = 10'000;
#else
        const int N = 10'000'000;
#endif

        string s;
        for (int i = 0; i < N; i++)
            s.push_back(RandInt32::get() % 2 + 'a');

        PROFILE_START(0);
        int dummy = 0;
        {
            OnlineZAlgorithm oz;
            for (int i = 0; i < int(s.length()); i++)
                oz.extend(s[i]);

            for (int i = 0; i < int(s.length()); i++)
                dummy += oz.query(i);
        }
        PROFILE_STOP(0);

        if (dummy == 0)
            cout << "dummy code" << endl;
    }
    cout << "OK!" << endl;
}
