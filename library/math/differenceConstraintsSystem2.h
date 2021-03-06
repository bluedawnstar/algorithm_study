#pragma once

/*
    1. Systems of Inequalities (system of difference constraints)

       Path p = (v1, v2, ..., vn)

          x[2] - x[1] <= w[1,2]
          x[3] - x[2] <= w[2,3]
          ...
          x[n] - x[n-1] <= w[n-1,n]

      <=>
                          n
          x[n] - x[1] <= SUM { w[i-1,i] } = w(p)
                         i=2

      ==>

          x[n] - x[1] <= (the weight of the shortest path from v1 to vn)
*/
struct DifferenceConstraintsSystem2 {
    int N;                                  // the number of variables
    vector<int> curValues;                  // current values of variables
    vector<int> maxValues;                  // max values of variables;
    vector<vector<pair<int, int>>> outEdges;// [u] = { (v, minW), ... }
    vector<vector<pair<int, int>>> inEdges; // [v] = { (u, maxW), ... }
    bool failed;

    DifferenceConstraintsSystem2(int n, const vector<int>& variablesMin, const vector<int>& variablesMax)
        : N(n), curValues(variablesMin), maxValues(variablesMax), outEdges(n), inEdges(n), failed(false) {
    }


    // adds a difference constraint
    //  :  minW <= v - u <= maxW  (u --> v)
    // 
    // returns false if this constraint led to a contradiction
    bool add(int u, int v, int minW, int maxW) {
        outEdges[u].push_back(make_pair(v, minW));  // u -> v <= minW
        inEdges[v].push_back(make_pair(u, maxW));   // v -> u <= maxW

        if (curValues[u] + minW > curValues[v]) {
            curValues[v] = curValues[u] + minW;
            if (!fix(v))
                return false;
        }

        if (curValues[v] - maxW > curValues[u]) {
            curValues[u] = curValues[v] - maxW;
            if (!fix(u))
                return false;
        }

        return true;
    }

    // returns 1) the solution to the constraint system
    //         2) or an empty vector if there is none
    vector<int> solve() {
        if (failed)
            return vector<int>();
        return curValues;
    }

private:
    bool fix(int variable) {
        if (curValues[variable] > maxValues[variable]) {
            failed = true;
            return false;
        }

        for (auto& it : outEdges[variable]) {
            int v = it.first;
            int minW = it.second;

            if (curValues[variable] + minW > curValues[v]) {
                curValues[v] = curValues[variable] + minW;
                if (!fix(v))
                    return false;
            }
        }

        for (auto& it : inEdges[variable]) {
            int u = it.first;
            int maxW = it.second;

            if (curValues[variable] - maxW > curValues[u]) {
                curValues[u] = curValues[variable] - maxW;
                if (!fix(u))
                    return false;
            }
        }

        return true;
    }
};
