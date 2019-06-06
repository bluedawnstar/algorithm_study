#pragma once

/*
       +---+
      /     \
     / upper \
    |     ....+
    +..../    |
     \ lower /
      \     /
       +---+

*/

// It is practically fast enough.
template <typename T>
struct DynamicConvexHull {
    set<pair<int, int>> upper;  // ordered by x coordinate (clockwise)
    set<pair<int, int>> lower;  // ordered by x coordinate (counter-clockwise)

    DynamicConvexHull() {
    }

    //--- build

    // O(N)
    void buildWithMutablePoints(vector<pair<int, int>>& points) {
        int j = 0, k = 0, n = int(points.size());
        sort(points.begin(), points.end());

        vector<pair<int, int>> tupper(n);
        vector<pair<int, int>> tlower(n);
        for (int i = 0; i < n; i++) {
            while (j >= 2 && cross(tlower[j - 2], tlower[j - 1], points[i]) <= 0)
                j--;
            while (k >= 2 && cross(tupper[k - 2], tupper[k - 1], points[i]) >= 0)
                k--;
            tlower[j++] = points[i];
            tupper[k++] = points[i];
        }
        tupper.resize(k);
        tlower.resize(j);

        for (auto p : tupper)
            upper.insert(p);
        for (auto p : tlower)
            lower.insert(p);
    }

    void build(vector<pair<int, int>> points) {
        buildWithMutablePoints(points);
    }

    //--- add

    // O(1) ~ O(N)
    bool addLeft(int x, int y) {
        if (int(lower.size()) <= 1) {
            upper.emplace(x, y);
            lower.emplace(x, y);
            return true;
        }
        
        if (lower.begin()->first <= x)
            return false;

        auto p = make_pair(x, y);

        auto itR = lower.begin();
        auto itL = itR++;
        while (itR != lower.end() && cross(p, *itL, *itR) <= 0) {
            lower.erase(itL);
            itL = itR++;
        }

        itR = upper.begin();
        itL = itR++;
        while (itR != upper.end() && cross(p, *itL, *itR) >= 0) {
            upper.erase(itL);
            itL = itR++;
        }

        lower.emplace(x, y);
        upper.emplace(x, y);

        return true;
    }

    // O(1) ~ O(N)
    bool addRight(int x, int y) {
        if (int(lower.size()) <= 1) {
            upper.emplace(x, y);
            lower.emplace(x, y);
            return true;
        }
        
        if (x <= lower.rbegin()->first)
            return false;

        auto p = make_pair(x, y);

        auto itL = lower.end();
        --itL;
        while (itL != lower.begin()) {
            auto itR = itL--;
            if (cross(*itL, *itR, p) > 0)
                break;
            lower.erase(itR);
        }

        itL = upper.end();
        --itL;
        while (itL != upper.begin()) {
            auto itR = itL--;
            if (cross(*itL, *itR, p) < 0)
                break;
            upper.erase(itR);
        }

        lower.emplace(x, y);
        upper.emplace(x, y);

        return true;
    }

    // O(logN) ~ O(NlogN)
    bool add(int x, int y) {
        if (int(lower.size()) < 2 || x < lower.begin()->first)
            return addLeft(x, y);
        else if (lower.rbegin()->first < x)
            return addRight(x, y);

        auto pL = *lower.begin();
        auto pR = *lower.rbegin();
        auto p = make_pair(x, y);
        if (1ll * (y - pL.second) * (pR.first - pL.first) >= 1ll * (x - pL.first) * (pR.second - pL.second)) {
            // upper envelope
            auto itR = upper.lower_bound(make_pair(x, y));
            auto itL = itR;
            if (itL == upper.begin())
                ++itR;
            else
                --itL;

            if (1ll * (y - itL->second) * (itR->first - itL->first) <= 1ll * (x - itL->first) * (itR->second - itL->second))
                return true;

            // left side
            while (itL != upper.begin()) {
                auto itL2 = itL--;
                if (cross(*itL, *itL2, p) < 0)
                    break;
                upper.erase(itL2);
            }

            // right side
            itL = itR++;
            while (itR != upper.end() && cross(p, *itL, *itR) >= 0) {
                upper.erase(itL);
                itL = itR++;
            }

            upper.emplace(x, y);
        } else {
            // lower envelope
            auto itR = lower.lower_bound(make_pair(x, y));
            auto itL = itR;
            if (itL == lower.begin())
                ++itR;
            else
                --itL;

            if (1ll * (y - itL->second) * (itR->first - itL->first) >= 1ll * (x - itL->first) * (itR->second - itL->second))
                return true;

            // left side
            while (itL != lower.begin()) {
                auto itL2 = itL--;
                if (cross(*itL, *itL2, p) > 0)
                    break;
                lower.erase(itL2);
            }

            // right side
            itL = itR++;
            while (itR != lower.end() && cross(p, *itL, *itR) <= 0) {
                lower.erase(itL);
                itL = itR++;
            }

            lower.emplace(x, y);
        }

        return true;
    }

    //--- get 

    vector<pair<int, int>> getUpper() const {
        vector<pair<int, int>> res;
        res.reserve(upper.size());
        for (auto it : upper)
            res.push_back(it);
        return res;
    }

    vector<pair<int, int>> getLower() const {
        vector<pair<int, int>> res;
        res.reserve(lower.size());
        for (auto it : lower)
            res.push_back(it);
        return res;
    }

    vector<pair<int, int>> getAllSorted() const {
        vector<pair<int, int>> res;
        if (int(upper.size()) < 2) {
            if (!upper.empty())
                res.push_back(*upper.begin());
            return res;
        }
        res.reserve(max(1, int(upper.size() + lower.size() - 2)));
        
        auto itLL = lower.begin();
        auto itUL = upper.begin(); ++itUL;
        auto itUR = upper.end(); --itUR;
        while (itLL != lower.end() && itUL != itUR) {
            if (*itLL < *itUL) {
                res.push_back(*itLL);
                ++itLL;
            } else {
                res.push_back(*itUL);
                ++itUL;
            }
        }
        if (itLL != lower.end()) {
            while (itLL != lower.end()) {
                res.push_back(*itLL);
                ++itLL;
            }
        } else {
            while (itUL != itUR) {
                res.push_back(*itUL);
                ++itUL;
            }
        }
        return res;
    }

    // return square distance, O(N)
    long long calcMaxDiameter() const {
        auto itJ = upper.begin();
        auto itM = upper.end(); --itM;
        auto itK = lower.end(); --itK;

        long long dist = 0;
        while (itJ != itM || itK != lower.begin()) {
            dist = max(dist, squareDist(*itJ, *itK));
            if (itJ == itM)
                --itK;
            else if (itK == lower.begin())
                ++itJ;
            else {
                auto j = itJ; ++j;
                auto k = itK; --k;
                if (1ll * (j->second - itJ->second) * (itK->first - k->first) > 1ll * (itK->second - k->second) * (j->first - itJ->first))
                    ++itJ;
                else
                    --itK;
            }
        }
        return dist;
    }

private:
    static long long cross(const pair<int, int>& P, const pair<int, int>& Q, const pair<int, int>& R) {
        return 1ll * (Q.first - P.first) * (R.second - P.second) - 1ll * (R.first - P.first) * (Q.second - P.second);
    }

    static long long squareDist(const pair<int, int>& L, const pair<int, int>& R) {
        long long x = L.first - R.first;
        long long y = L.second - R.second;
        return x * x + y * y;
    }
};
