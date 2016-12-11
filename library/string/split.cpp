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


///////////////////////////////////////////////////////////////////////////////

#include <iostream>

static ostream& operator <<(ostream& os, vector<string>& rhs) {
    cout << "(";
    if (!rhs.empty())
        cout << "\"" << rhs[0] << "\"";
    for (int i = 1; i < (int)rhs.size(); i++)
        cout << ", " << "\"" << rhs[i] << "\"";
    cout << ")";

    return os;
}

void testSplit() {
    return; // skip this test...

    cout << "-- split() ---------" << endl;
    cout << split("a.b.c..", '.') << endl;
    cout << split(".b.c..", '.') << endl;
    cout << split(".b.c..d", '.') << endl;
}
