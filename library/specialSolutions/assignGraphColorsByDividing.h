#pragma once

// https://www.codechef.com/problems/DOOFST

// Assigning colors repeatedly by dividing into two groups
struct AssignGraphColorsByDividing {
    int N;
    vector<vector<int>> edges;
    vector<int> degree;

    //--- result

    vector<int> nodeColor;  // node nodeColor
    vector<int> colorCount; // the number of nodeColor

    AssignGraphColorsByDividing() {
    }

    explicit AssignGraphColorsByDividing(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
        degree.assign(N, 0);
        nodeColor.assign(N, 0);
        colorCount.assign(N, 0);
    }

    // add a divisible pair 
    void add(int u, int v) {
        if (u < v)
            swap(u, v);
        edges[u].push_back(v);
        ++degree[u];
        ++degree[v];
    }

    bool assignColor() {
        vector<int> res(N);

        int maxColor = 0;
        for (int u = 0; u < N; u++) {
            // find the lowest nodeColor different from neighbor nodes
            vector<bool> used(N);
            for (auto v : edges[u])
                used[nodeColor[v]] = true;

            int color = 0;
            while (used[color])
                color++;

            nodeColor[u] = color;
            colorCount[color]++;
        }

        // check if it can be divided into two groups
        //  - nodes that have the same color as u
        //  - nodes that do not
        for (int u = 0; u < N; u++) {
            if (degree[u] != N - colorCount[nodeColor[u]])
                return false;
        }

        return true;
    }
};
