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

#include "ahoCorasick.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testAhoCorasick() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Aho-Corasick -------------------------" << endl;
    {
        AhoCorasick<> trie;

        trie.insert("she", 0);
        trie.insert("he", 1);
        trie.insert("hers", 2);
        trie.insert("his", 3);
        trie.build();

        //                             012345678
        auto ans = trie.doAhoCorasick("hershishe");
        sort(ans.begin(), ans.end());

        vector<pair<int, int>> gt{ { 1, 1 }, { 3, 2 }, { 6, 3 }, { 8, 0 }, { 8, 1 } };
        assert(ans == gt);
    }
    {
        AhoCorasick<> trie;

        trie.insert("she", 0);
        trie.insert("he", 1);
        trie.insert("hers", 2);
        trie.insert("his", 3);
        trie.build();

        //                012345678
        const string s = "hershishe";

        vector<vector<int>> gt{
            { },
            { 1 },
            { 1 },
            { 1, 2 },
            { 1, 2 },
            { 1, 2 },
            { 1, 2, 3 },
            { 1, 2, 3 },
            { 1, 2, 3, 1, 0 },
        };

        vector<int> ans;

        auto* curr = trie.firstAhoCorasick(s[0], ans);

        for (int i = 1; i < int(s.length()); i++) {
            curr = trie.nextAhoCorasick(curr, s[i], ans);
            assert(ans == gt[i]);
        }
    }

    cout << "OK!" << endl;
}
