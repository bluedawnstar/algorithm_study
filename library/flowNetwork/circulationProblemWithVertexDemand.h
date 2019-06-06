#pragma once

#include "maxFlowDinic.h"

// https://en.wikipedia.org/wiki/Circulation_problem
// https://www.win.tue.nl/~nikhil/courses/2013/2WO08/scribenotes26febv02.pdf
// https://koosaga.com/134
// https://github.com/koosaga/DeobureoMinkyuParty/blob/master/teamnote.pdf
// https://www.cs.cmu.edu/~ckingsf/bioinfo-lectures/flowext.pdf

template <typename T, const T INF = 0x3f3f3f3f>
struct CirculationProblemWithVertexDemand {
    int N;                      // vertex count
    MaxFlowDinic<T, INF> mf;    // 0: source, 1: sink
    T plusD, minusD;

    CirculationProblemWithVertexDemand() {
    }

    void init(const vector<T>& vertexDemand) {
        N = int(vertexDemand.size());
        mf.init(N + 2);

        plusD = 0;
        minusD = 0;
        for (int u = 0; u < N; u++) {
            if (vertexDemand[u] < 0) {
                mf.addEdge(0, u + 2, -vertexDemand[u], 0);
                minusD += -vertexDemand[u];
            } else if (vertexDemand[u] > 0) {
                mf.addEdge(u + 2, 1, vertexDemand[u], 0);
                plusD += vertexDemand[u];
            }
        }
    }

    void addEdge(int u, int v) {
        mf.addEdge(u + 2, v + 2, INF, 0);
    }

    //--- solve

    bool isPossible() {
        if (minusD != plusD)
            return false;
        return mf.calcMaxFlow(0, 1) == plusD;
    }
};
