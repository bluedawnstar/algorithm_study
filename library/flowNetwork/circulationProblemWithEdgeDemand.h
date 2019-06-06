#pragma once

#include "maxFlowDinic.h"

// https://en.wikipedia.org/wiki/Circulation_problem
// https://www.win.tue.nl/~nikhil/courses/2013/2WO08/scribenotes26febv02.pdf
// https://koosaga.com/134
// https://github.com/koosaga/DeobureoMinkyuParty/blob/master/teamnote.pdf
// https://www.cs.cmu.edu/~ckingsf/bioinfo-lectures/flowext.pdf

// Circulation problem with edge demand
// Max-flow with edge demand (a.k.a L-R max-flow)

template <typename T, const T INF = 0x3f3f3f3f>
struct CirculationProblemWithEdgeDemand {
    MaxFlowDinic<T, INF> mf;    // 0: source, 1: sink
    T lowerBoundSum;

    CirculationProblemWithEdgeDemand() {
    }

    void init(int n) {
        mf.init(n + 2);
        lowerBoundSum = 0;
    }

    void addEdge(int u, int v, T lowerBound, T capacity) {
        lowerBoundSum += lowerBound;
        mf.addEdge(u + 2, v + 2, capacity - lowerBound, 0);
        mf.addEdge(0, v + 2, lowerBound, 0);
        mf.addEdge(u + 2, 1, lowerBound, 0);    }

    void addEdge(int u, int v, T lowerBound, T capacity, int edgeIndex) {
        lowerBoundSum += lowerBound;
        mf.addEdge(u + 2, v + 2, capacity - lowerBound, 0, edgeIndex);
        mf.addEdge(0, v + 2, lowerBound, 0);
        mf.addEdge(u + 2, 1, lowerBound, 0);    }

    //--- solve

    // return (possibility, max-flow)
    pair<bool,T> solve() {
        auto flow = mf.calcMaxFlow(0, 1);
        return make_pair((flow == lowerBoundSum), flow);
    }

    // return (possibility, max-flow from u to v)
    pair<bool,T> solve(int u, int v) {
        mf.addEdge(v + 2, u + 2, INF, 0);   // to reduce as maxflow with lower bounds
        if (mf.calcMaxFlow(0, 1) != lowerBoundSum)
            return make_pair(false, T(0));
        return make_pair(true, mf.calcMaxFlow(u + 2, v + 2));
    }
};
