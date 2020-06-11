#pragma once

#include "minCutMaxFlowDinic.h"

// https://www.wikiwand.com/en/Max-flow_min-cut_theorem#/image_segmentation_problem


// https://www.codechef.com/problems/NRWP
/*
  1. given condition
      when each node has p[i] and h[i] values,

      F = SUM p[i] * h[i] +  SUM p[i] * p[j]
           i                (i,j)

        - h[i] is positive or negative value
        - |p[i]| is given

  2. problem
      Determine the sign of |p[i]| so that F is maximum

      F =  SUM p[i] * h[i] +  SUM p[i] * p[j]
            i in V           (i,j) in E

        =  SUM |p[i] * h[i]| + SUM |p[i] * p[j]|
            i in V            (i,j) in E

          - 2 * { SUM |p[i] * h[i]|
                   i in S, h[i] is negative value

                + SUM |p[i] * h[i]|
                   i in T, h[i] is positive value

                + SUM |p[i] * p[j]| }
                   i in S, j in T

        =  SUM |p[i] * h[i]| + SUM |p[i] * p[j]|
            i in V            (i,j) in E

           - 2 * min-cut

         S is a set of nodes to have positive p value
         T is a set of nodes to have negative p value


     <nodes to have  <nodes to have
      positive h>     negative h>

        p[i] >= 0      p[j] <= 0
     --------------- --------------

        /      h_p ... h_n     \
      s - ...  h_p ... h_n ... - t
        \      h_p ... h_n     /
               h_p ... h_n

        h_p : positive h
        h_n : negative n

    <solution>
     - step1 : calculate min-cut and calculate max F
     - step2 : assign nodes to be connected to 's' to plus sign group
               assign nodes to be connected to 't' to minus sign group
*/
template <typename T, const T INF = 0x3f3f3f3f>
struct SignAssignmentProblem : public MinCutMaxFlowDinic<T, INF> {
    vector<T> h;                // positive or negative value
    vector<T> p;                // positive value
    vector<int> sign;           // 'sign[i]' is a sign of 'p[i]' (-1 or +1)

    int s, t;
    T absSum;

    void init(int n, const vector<T>& h, const vector<T>& p) {
        MinCutMaxFlowDinic<T, INF>::init(n + 2);

        this->h = h;
        this->p = p;
        sign.assign(n, 1);

        s = n;
        t = n + 1;
        absSum = 0;
    }

    void addEdge(int u, int v) {
        T cap = p[u] * p[v];
        MinCutMaxFlowDinic<T, INF>::addEdge(u, v, cap, cap);
        absSum += cap;
    }

    T solve() {
        // link s to nodes to have positive 'h' values
        // link t to nodes to have negative 'h' values
        for (int i = 0; i + 2 < N; i++) {
            T cap = p[i] * abs(h[i]);
            if (h[i] > 0)
                MinCutMaxFlowDinic<T, INF>::addEdge(i, s, cap, cap);
            else
                MinCutMaxFlowDinic<T, INF>::addEdge(i, t, cap, cap);
            absSum += cap;
        }

        T res = absSum - 2 * MinCutMaxFlowDinic<T, INF>::calcMinCut(s, t);

        vector<int> groupS, groupT;
        MinCutMaxFlowDinic<T, INF>::findGroup(groupS, groupT);

        for (auto x : groupT) {
            if (x + 2 < N)
                sign[x] = -1;
        }

        return res;
    }
};
