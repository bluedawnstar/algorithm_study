#include <cmath>
#include <functional>
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;

#include "gaussianElimination.h"
#include "gaussianEliminationMod.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

const int MOD = 1'000'000'007;

void testGaussianElimination() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Gaussian Elimination -----------------------" << endl;
    {
        auto ans = SLAE::gauss(Matrix<double>(vector<vector<double>>{ { 4, 2, -1 }, { 2, 4, 3 }, { -1, 3, 5 } }), vector<double>{ 5, 19, 20 });
        auto ans2 = SLAEMod<MOD>::gauss(MatrixMod<MOD>(vector<vector<int>>{ { 4, 2, MOD - 1 }, { 2, 4, 3 }, { MOD - 1, 3, 5 } }), vector<int>{ 5, 19, 20 });
        cout << ans << endl;
        cout << ans2 << endl;
        assert(int(ans[0] + 0.5) == 1);
        assert(int(ans[1] + 0.5) == 2);
        assert(int(ans[2] + 0.5) == 3);
        assert(ans2[0] == 1);
        assert(ans2[1] == 2);
        assert(ans2[2] == 3);
    }
    {
        vector<double> ans;
        int cnt = SLAE::gaussGeneral(vector<vector<double>>{ { 4, 2, -1 }, { 2, 4, 3 }, { -1, 3, 5 } }, vector<double>{ 5, 19, 20 }, ans);
        cout << "cnt = " << cnt << ", ans = " << ans << endl;
        assert(cnt == 1);
        assert(int(ans[0] + 0.5) == 1);
        assert(int(ans[1] + 0.5) == 2);
        assert(int(ans[2] + 0.5) == 3);
    }
    {
        vector<double> ans;
        int cnt = SLAE::gaussGeneral(vector<vector<double>>{ { 4, 2, 0, -1 }, { 2, 4, 0, 3 }, { -1, 3, 0, 5 } }, vector<double>{ 5, 19, 20 }, ans);
        cout << "cnt = " << cnt << ", ans = " << ans << endl;
        assert(cnt == SLAE::INF);
        assert(int(ans[0] + 0.5) == 1);
        assert(int(ans[1] + 0.5) == 2);
        assert(int(ans[3] + 0.5) == 3);
    }
    {
        vector<double> ans;
        int cnt = SLAE::gaussGeneral(vector<vector<double>>{ { 4, 2, -1 }, { 2, 4, 3 }, { -1, 3, 5 }, { 1, 2, 3 } }, vector<double>{ 5, 19, 20, 40 }, ans);
        cout << "cnt = " << cnt << ", ans = " << ans << endl;
        assert(cnt == 0);
    }
    {
        vector<double> ans;
        int cnt = SLAE::gaussGeneral(vector<vector<double>>{ { 4, 2, -1 }, { 2, 4, 3 }, { 2, 4, 3 } }, vector<double>{ 5, 19, 20 }, ans);
        cout << "cnt = " << cnt << ", ans = " << ans << endl;
        assert(cnt == 0);
    }
    {
        vector<double> ans;
        int cnt = SLAE::gaussGeneral(vector<vector<double>>{ { 4, 2, -1 }, { 2, 4, 3 } }, vector<double>{ 5, 19 }, ans);
        cout << "cnt = " << cnt << ", ans = " << ans << endl;
        assert(cnt == SLAE::INF);
    }
    {
        bitset<3ull> ans;
        vector<bitset<3ull>> a(3);
        a[0][0] = 1; a[0][1] = 1; a[0][2] = 0;
        a[1][0] = 0; a[1][1] = 1; a[1][2] = 0;
        a[2][0] = 0; a[2][1] = 0; a[2][2] = 1;
        bitset<3ull> b;
        b[0] = 1; b[1] = 0; b[2] = 1;

        auto cnt = SLAE::gaussMod2(a, b, 3, 3, ans);
        cout << "cnt = " << cnt << ", ans = " << ans[0] << ans[1] << ans[2] << endl;
        assert(cnt == 1);
        assert(int(ans[0]) == 1);
        assert(int(ans[1]) == 0);
        assert(int(ans[2]) == 1);
    }
    {
        bitset<3ull> ans;
        vector<bitset<3ull>> a(3);
        a[0][0] = 1; a[0][1] = 1; a[0][2] = 0;
        a[1][0] = 0; a[1][1] = 1; a[1][2] = 0;
        bitset<3ull> b;
        b[0] = 1; b[1] = 0;;

        auto cnt = SLAE::gaussMod2(a, b, 2, 3, ans);
        cout << "cnt = " << cnt << ", ans = " << ans[0] << ans[1] << ans[2] << endl;
        assert(cnt == 2);
        assert(int(ans[0]) == 1);
        assert(int(ans[1]) == 0);
    }
    cout << "OK!" << endl;
}
