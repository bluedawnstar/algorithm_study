#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//#### Implementation doesn't complete.

struct Decimal {
    vector<char> val;

    string add(string a, string b) {
        string res;

        int carry = 0;
        for (int i = int(a.length()) - 1, j = int(b.length()) - 1; i >= 0 || j >= 0; i--, j--) {
            carry += (i >= 0 ? a[i] - '0' : 0) + (j >= 0 ? b[j] - '0' : 0);
            res.push_back(carry % 10 + '0');
            carry /= 10;
        }
        if (carry)
            res.push_back(carry + '0');

        reverse(res.begin(), res.end());

        return res;
    }

    string sub(string a, string b) {
        string res;

        int carry = 0;
        for (int i = int(a.length()) - 1, j = int(b.length()) - 1; i >= 0 || j >= 0; i--, j--) {
            carry = (i >= 0 ? a[i] - '0' : 0) - (j >= 0 ? b[j] - '0' : 0) + carry;
            res.push_back(abs(carry) % 10 + '0');
            carry /= 10;
        }
        if (carry)
            res.push_back('-');

        reverse(res.begin(), res.end());

        return res;
    }

    string mult(string a, string b) {
        string res;

        string premult[10];
        premult[0] = "0";
        premult[1] = a;
        for (int i = 2; i < 10; i++)
            premult[i] = add(premult[i - 1], a);

        int shift = 0;
        for (int i = int(b.length()) - 1; i >= 0; i--) {
            if (b[i] != '0') {
                string t = premult[b[i] - '0'];
                if (shift > 0)
                    t.append(shift, '0');
                res = add(res, t);
            }
            shift++;
        }

        return res;
    }

    string pow2(int n) {
        if (n == 0)
            return "1";
        else if (n == 1)
            return "2";

        string res = mult(pow2(n / 2), pow2(n / 2));
        if (n & 1)
            res = mult(res, pow2(1));

        return res;
    }


    static string makePalindromic(string s) {
        for (int i = 0, j = int(s.length()) - 1; i < j; i++, j--)
            s[j] = s[i];
        return s;
    }

    static int compare(const string& s1, const string& s2) {
        if (s1.length() < s2.length())
            return -1;
        else if (s1.length() > s2.length())
            return 1;

        for (int i = 0; i < int(s1.length()); i++) {
            if (s1[i] < s2[i])
                return -1;
            else if (s1[i] > s2[i])
                return 1;
        }
        return 0;
    }

    static string difference(const string& s1, const string& s2) {
        int c = compare(s1, s2);
        if (c == 0)
            return string("0");

        const string& lo = (c < 0) ? s1 : s2;
        const string& hi = (c > 0) ? s1 : s2;

        string res;
        res.reserve(hi.length());

        int carry = 0;
        for (int i = int(hi.length()) - 1, j = int(lo.length()) - 1; i >= 0 || j >= 0; i--, j--) {
            carry = (i >= 0 ? hi[i] - '0' : 0) - (j >= 0 ? lo[j] - '0' : 0) + carry;
            res.push_back(abs(carry) % 10 + '0');
            carry /= 10;
        }
        reverse(res.begin(), res.end());

        return res;
    }
};


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testDecimal() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Decimal ---" << endl;
    // ...
}
