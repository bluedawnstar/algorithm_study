#pragma once

struct PalindromicNumberLongLong {
    static string makePalindromic(string s) {
        for (int i = 0, j = int(s.length()) - 1; i < j; i++, j--)
            s[j] = s[i];
        return s;
    }

    static string nearestPalindromic(const string& n) {
        if (n == "0")
            return "1";

        long long orgVal = stoll(n);

        // candidate #1 (ex: 123xx -> 12321, 123xxx -> 123321)
        string res = makePalindromic(n);
        long long resVal = stoll(res);
        long long diff = abs(resVal - orgVal);

        long long scale = (long long)pow(10, int(n.length()) / 2);

        // candidate #2 (ex: 123xx -> 12221, 123xxx -> 122221, 100xx -> 9999)
        string smaller = makePalindromic(to_string((orgVal / scale) * scale - 1));
        // candidate #3 (ex: 123xx -> 12421, 123xxx -> 124421, 99xx -> 10001)
        string bigger = makePalindromic(to_string((orgVal / scale) * scale + scale));

        long long smallerVal = stoll(smaller);
        if (diff == 0 || abs(orgVal - smallerVal) <= diff) {
            res = smaller;
            diff = abs(orgVal - smallerVal);
        }

        long long biggerVal = stoll(bigger);
        if (abs(orgVal - biggerVal) < diff)
            res = bigger;

        return res;
    }
};

struct PalindromicNumber {
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

    static string add(const string& a, const string& b) {
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

    static string subAbs(const string& s1, const string& s2) {
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
            if (carry < 0) {
                res.push_back(carry + 10 + '0');
                carry = -1;
            } else {
                res.push_back(carry + '0');
                carry = 0;
            }
        }
        while (res.back() == '0' && int(res.length()) > 1)
            res.pop_back();
        reverse(res.begin(), res.end());

        return res;
    }

    static string shift10(const string& s, int n) {
        if (n == 0)
            return s;

        string res = s;
        if (n > 0)
            return res.append(n, '0');
        else
            return res.erase(int(s.length()) + n);
    }

    static string nearestPalindromic(const string& n) {
        if (n == "0")
            return "1";

        // candidate #1 (ex: 123xx -> 12321, 123xxx -> 123321)
        string res = makePalindromic(n);
        string diff = subAbs(res, n);

        int scaleN = int(n.length()) / 2;
        string scale = shift10("1", scaleN);

        // candidate #2 (ex: 123xx -> 12221, 123xxx -> 122221, 100xx -> 9999)
        string smaller = makePalindromic(subAbs(shift10(shift10(n, -scaleN), scaleN), "1"));
        // candidate #3 (ex: 123xx -> 12421, 123xxx -> 124421, 99xx -> 10001)
        string bigger = makePalindromic(add(shift10(shift10(n, -scaleN), scaleN), scale));

        if (diff == "0" || compare(subAbs(n, smaller), diff) <= 0) {
            res = smaller;
            diff = subAbs(n, smaller);
        }

        if (compare(subAbs(n, bigger), diff) < 0)
            res = bigger;

        return res;
    }
};
