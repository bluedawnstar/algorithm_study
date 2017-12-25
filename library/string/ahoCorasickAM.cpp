#include <memory.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "ahoCorasickAM.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testAhoCorasickAM() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Array Mapped Aho-Corasick -------------------------" << endl;
    {
        AhoCorasickAM trie;

        trie.insert("SHE", 0);
        trie.insert("HE", 1);
        trie.insert("HERS", 2);
        trie.insert("HIS", 3);
        trie.build();

        //                             012345678
        auto ans = trie.doAhoCorasick("HERSHISHE");
        sort(ans.begin(), ans.end());
        
        vector<pair<int, int>> gt{ { 1, 1 }, { 3, 2 }, { 6, 3 }, { 8, 0 }, { 8, 1 } };
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
