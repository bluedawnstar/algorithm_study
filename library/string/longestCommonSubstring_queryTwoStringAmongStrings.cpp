#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "suffixArray.h"
#include "lcpArraySparseTable.h"

//
// https://www.hackerrank.com/contests/w30/challenges/substring-queries
// 

struct LongestCommonStringLength {
    int mN;                                             // the number of strings
    int mQ;                                             // the number of queries
    vector<pair<int, int>> mS;
    string mSS;

    vector<int> mSuffixToStrID;

    vector<int> mSA;
    vector<int> mLCP;

    vector<int> mAns;
    vector<pair<int, int>> mQIn;
    unordered_map<int, unordered_map<int, int>> mQIndex;// (L, R) -> Q index
    vector<unordered_map<int, int>> mQuery;             // R --> (L, Q index)

    // 
    // 1) how to reduce redundant questions
    //      mQuery[R][L] = the index of answer to (L, R) or (R, L)
    // 2) how to reduce the number of comparing between two suffixes 
    //      update forward and backward instead of making all pairs
    // 3) how to use suffix array, LCP array, sparse table, etc
    //      
    vector<int> solve(const vector<string>& strs, const vector<pair<int,int>>& query) {
        mN = (int)strs.size();
        mQ = (int)query.size();

        int totLen = 0;
        for (const string& s : strs)
            totLen += (int)s.length();
        mSuffixToStrID.resize(mN + totLen + 1);

        mS.clear();
        mSS.clear();
        mS.reserve(mN);
        for (int i = 0; i < mN; i++) {
            mS.emplace_back(mSS.length(), mSS.length() + strs[i].length());

            int len = (int)mSS.length();
            mSS += strs[i];
            mSS += char('z' + 1);

            for (int j = 0; j < (int)strs[i].length(); j++)
                mSuffixToStrID[len + j] = i;
            mSuffixToStrID[len + strs[i].length()] = -1;
        }

        mAns.resize(mQ);
        mQIn.resize(mQ);
        mQuery.resize(mN);   // R --> (L, Q index)

        // grouping queries for speed up
        for (int i = 0; i < mQ; i++) {
            int L = query[i].first;
            int R = query[i].second;

            if (L > R)
                swap(L, R);

            mQIn[i] = make_pair(L, R);
            if (mQIndex[L].find(R) == mQIndex[L].end())
                mQIndex[L][R] = i;

            if (L == R)
                mAns[mQIndex[L][R]] = (int)mS[L].second - (int)mS[L].first;
            else
                mQuery[R][L] = mQIndex[L][R];
        }

        // making suffix array, LCP array, sparse table for LCP array
        mSA = makeSuffixArrayFast(mSS);
        mLCP = makeLcpArray(mSA, mSS, (int)mSS.length());
        LcpArraySparseTable sparseTable(mLCP, (int)mLCP.size());

        // check forward
        vector<int> lastSAIndex(mN, -1);
        int prevStrID = -1;
        for (int i = 0; i < (int)mSA.size(); i++) {
            int rStrID = mSuffixToStrID[mSA[i]];
            if (rStrID < 0)
                continue;

            if (prevStrID != rStrID) {
                for (auto it : mQuery[rStrID]) {
                    int lStrID = it.first;
                    if (lastSAIndex[lStrID] < 0)
                        continue;

                    int lcp = sparseTable.lcp(lastSAIndex[lStrID], i);

                    int maxLen = min(mS[lStrID].second - mSA[lastSAIndex[lStrID]],
                                     mS[rStrID].second - mSA[i]);
                    if (lcp > maxLen)
                        lcp = maxLen;

                    mAns[it.second] = max(mAns[it.second], lcp);
                }
            }
            lastSAIndex[rStrID] = i;
            prevStrID = rStrID;
        }

        // check backward
        fill(lastSAIndex.begin(), lastSAIndex.end(), -1);
        prevStrID = -1;
        for (int i = (int)mSA.size() - 1; i >= 0; i--) {
            int rStrID = mSuffixToStrID[mSA[i]];
            if (rStrID < 0)
                continue;

            if (prevStrID != rStrID) {
                for (auto it : mQuery[rStrID]) {
                    int lStrID = it.first;
                    if (lastSAIndex[lStrID] < 0)
                        continue;

                    int lcp = sparseTable.lcp(i, lastSAIndex[lStrID]);

                    int maxLen = min(mS[lStrID].second - mSA[lastSAIndex[lStrID]],
                                     mS[rStrID].second - mSA[i]);
                    if (lcp > maxLen)
                        lcp = maxLen;

                    mAns[it.second] = max(mAns[it.second], lcp);
                }
            }
            lastSAIndex[rStrID] = i;
            prevStrID = rStrID;
        }

        // making answer
        vector<int> res(mQ);
        for (int i = 0; i < mQ; i++)
            res[i] = mAns[mQIndex[mQIn[i].first][mQIn[i].second]];

        return res;
    }

private:
    vector<int> makeSuffixArrayFast(const string& s) {
        int n = (int)s.length();

        vector<int> SA(n);
        if (n <= 1)
            return SA;

        int m = 27;                                     // caution: alphabet + delemiter
        vector<int> cnt(max(n, m)), currG(n), nextG(n);

        for (int i = 0; i < n; i++) {
            currG[i] = s[i] - 'a';
            ++cnt[currG[i]];
        }
        for (int i = 1; i < m; i++)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--)
            SA[--cnt[currG[i]]] = i;

        int len = 1;
        do {
            int g = 0;
            for (int i = n - len; i < n; i++)
                nextG[g++] = i;
            for (int i = 0; i < n; i++) {
                if (SA[i] >= len)
                    nextG[g++] = SA[i] - len;
            }

            fill(cnt.begin(), cnt.begin() + m, 0);
            for (int i = 0; i < n; i++)
                ++cnt[currG[nextG[i]]];
            for (int i = 1; i < m; i++)
                cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--)
                SA[--cnt[currG[nextG[i]]]] = nextG[i];

            int curSA = SA[0];
            nextG[curSA] = g = 0;
            for (int i = 1; i < n; i++) {
                int prvSA = curSA;
                curSA = SA[i];

                nextG[curSA] = (prvSA + len < n && curSA + len < n
                    && currG[prvSA] == currG[curSA]
                    && currG[prvSA + len] == currG[curSA + len])
                    ? g : ++g;
            }
            swap(currG, nextG);

            len <<= 1;
            m = g + 1;
        } while (m < n);

        return SA;
    }
};


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"


void testLongestCommonSubstring_queryTwoStringAmongStrings() {
    //return; //TODO: if you want to test string functions, make this line to a comment.

    cout << "-- Testing for Longest Common Substring Length Queries -------------" << endl;

    vector<string> s1 = {
        "vyemcyjagcttykawuoixitmucywoqkyweqmfonmrsimgifciaesivodsleovqeyaownngwizswmgaaluvryeaosznwuugonwfmasewaqesqcacqievglnyk",
        "mkascndgjaqmzqgghkgkqcikixyaaimcguqukgyzywmxgjcfqnwsawfgusscwkucvmyijvqsiscbiqcanmayerpausgiaabeskmyocjaewrplmqawmufwyhwcvcagviswqmuomanitkkzargkocsaguacasiqoboavfifhoaqgbmglzueksomxouucybzmazggakvokvvaauggjnobcaxfxaooeqswiwwsnmylamwkjwluqyaueyakooeiilzahvmcaqabwxyqisayoljhwogyqamuajgquacxogkihsaaveibymqaamtuheofwcarfuasdgewqartwakaimtwfkkanwenumsywuwswarqkbqoygewkamnyosghiueoyyoueyniizagondaseqenasmecfwgikraalgecvtmmvcuagimnoqemnrtkskcshunhkibgqjluagwysgtsgqwopypwmdiewawtmyisauostxjquwaaafqyswixtyeaakezjnoluanpkpckcgisaekiedaaeqarmcgsapdkiaecoskysancgjciahyaevscoosoawilxykzuyeadeupcuyuuqaaqpcmyfatawusyezzvnalebveomotkcsmsfwaigyojmintquqmwbyexyuviopgyeacguoasfuyaezwemekmmvugkikucwcwwgyiesavoaugwswaxalqczqaabnmovgimqutwhaaguaauytipkyeeywxgienornltchyaaycauu",
        "kgaylee",
        "a",
        "ausew",
        "mqugrs",
        "a",
        "m",
        "w",
        "a",
        "odg",
        "k",
        "y",
        "v",
        "a",
        "g",
        "g",
        "w",
        "m",
        "d",
        "a",
        "z",
        "k",
        "v",
        "s",
        "q",
        "f",
        "v",
        "w",
        "g",
        "f",
        "n",
        "a",
        "i",
        "g",
        "m",
        "o",
        "j",
        "g",
        "e",
        "a",
        "i",
        "a",
        "q",
        "q",
        "i",
        "y",
        "n",
        "v",
        "o",
        "a",
        "n",
        "a",
        "d",
        "w",
        "i",
        "u",
        "o",
        "i",
        "j",
        "l",
        "w",
        "k",
        "d",
        "j",
        "q",
        "f",
        "c",
        "m",
        "c",
        "o",
        "y",
        "g",
        "i",
        "u",
        "u",
        "q",
        "s",
        "o",
        "i",
        "t",
        "m",
        "c",
        "j",
        "y",
        "a",
        "k",
        "i",
        "s",
        "p",
        "b",
        "p",
        "y",
        "e",
        "c",
        "u",
        "k",
        "q",
        "t",
        "q"
    };
    vector<pair<int, int>> q1 = {
        { 49, 12 },
        { 29, 44 },
        { 32,  3 },
        { 39, 13 },
        { 76, 71 },
        { 43, 47 },
        { 46, 67 },
        { 50, 53 },
        { 95, 10 },
        { 11, 78 },
        { 14, 23 },
        { 82, 37 },
        { 39, 25 },
        { 21, 78 },
        { 81, 55 },
        { 42, 71 },
        { 47, 34 },
        { 19, 23 },
        { 59, 58 },
        { 95, 35 },
        { 96, 33 },
        { 17, 70 },
        { 64, 28 },
        {  9, 70 },
        {  1, 78 },
        { 54, 19 },
        { 61, 96 },
        { 22, 32 },
        { 71, 26 },
        { 17, 20 },
        { 84, 90 },
        { 44, 67 },
        { 70, 38 },
        {  4, 21 },
        { 20, 82 },
        { 46, 86 },
        { 62, 92 },
        { 36, 62 },
        {  2, 43 },
        { 23,  2 },
        { 71, 74 },
        { 39, 64 },
        { 33, 25 },
        { 42, 96 },
        {  9, 72 },
        { 21,  4 },
        {  3, 95 },
        { 31, 46 },
        { 97, 70 },
        { 39, 94 },
        { 61, 18 },
        { 49, 81 },
        { 54, 95 },
        { 76, 84 },
        {  3,  8 },
        { 66, 77 },
        { 73, 40 },
        { 91, 33 },
        { 26, 10 },
        { 26, 97 },
        {  5, 31 },
        { 26, 70 },
        {  8, 74 },
        { 56, 71 },
        { 56,  8 },
        { 50, 28 },
        { 64, 58 },
        { 84,  9 },
        { 64, 60 },
        { 16, 19 },
        {  7, 31 },
        { 35, 67 },
        { 59, 21 },
        { 70, 69 },
        { 33, 35 },
        { 14, 78 },
        { 92, 31 },
        {  0, 81 },
        {  5, 75 },
        { 59, 86 },
        { 19, 62 },
        { 99, 60 },
        { 32, 78 },
        { 50, 66 },
        { 97, 66 },
        { 41, 44 },
        { 47, 72 },
        { 14, 79 },
        { 22, 93 },
        {  7, 40 },
        { 72, 68 },
        { 98, 88 },
        { 77,  4 },
        { 10, 90 },
        { 22, 34 },
        {  0, 68 },
        { 35,  7 },
        { 83, 57 },
        { 66, 70 },
        { 97, 55 },
    };
    vector<int> ans1 = {
        { 0 },
        { 0 },
        { 1 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 1 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 1 },
        { 1 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 1 },
        { 0 },
        { 0 },
        { 1 },
        { 1 },
        { 0 },
        { 0 },
        { 0 },
    };

    LongestCommonStringLength lcs;
    assert(lcs.solve(s1, q1) == ans1);

    cout << "OK!" << endl;
}
