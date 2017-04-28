#include <string>
#include <vector>

using namespace std;

vector<string> split(string s, char c) {
    int i, j = 0;
    vector<string> res;
    for (i = 0; i < (int)s.length(); i++) {
        if (s[i] == c) {
            res.push_back(s.substr(j, i - j));
            j = i + 1;
        }
    }
    res.push_back(s.substr(j, i - j));

    return res;
}

// (start position, length)
vector<pair<int,int>> split2(string s, char c) {
    int i, j = 0;
    vector<pair<int,int>> res;
    for (i = 0; i < (int)s.length(); i++) {
        if (s[i] == c) {
            res.push_back(make_pair(j, i - j));
            j = i + 1;
        }
    }
    res.push_back(make_pair(j, i - j));

    return res;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <iostream>
#include "../common/iostreamhelper.h"

void testSplit() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- split() ---------" << endl;
    cout << split("a.b.c..", '.') << endl;
    cout << split(".b.c..", '.') << endl;
    cout << split(".b.c..d", '.') << endl;

    cout << "-- split2() ---------" << endl;
    cout << split2("a.b.c..", '.') << endl;
    cout << split2(".b.c..", '.') << endl;
    cout << split2(".b.c..d", '.') << endl;
}
