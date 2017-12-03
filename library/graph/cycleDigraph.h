#pragma once

// digraph
struct CycleDetector {
    vector<int> cycleParent;    // cycle group

    void detect(vector<vector<int>>& edges, int N, int root) {
        cycleParent.assign(N, -1);

        visited.assign(N, false);
        inStack.assign(N, false);

        stack.clear();
        stack.reserve(N);

        dfs(edges, root);
    }

    int findCycleSet(int u) {
        if (cycleParent[u] < 0)
            return -1;  // no cycle

        if (cycleParent[u] == u)
            return u;

        return cycleParent[u] = findCycleSet(cycleParent[u]);
    }

private:
    vector<bool> visited;
    vector<bool> inStack;
    vector<int> stack;

    // find cycle groups
    void dfs(vector<vector<int>>& edges, int u) {
        visited[u] = true;
        inStack[u] = true;
        stack.push_back(u);
        for (int v : edges[u]) {
            if (!visited[v]) {
                dfs(edges, v);
            } else if (inStack[v]) {
                for (int i = (int)stack.size() - 1; i >= 0; i--) {
                    if (stack[i] == v) {
                        if (cycleParent[v] < 0)
                            cycleParent[v] = v;
                        break;
                    }
                    cycleParent[stack[i]] = v;
                }
            }
        }
        stack.pop_back();
        inStack[u] = false;
    }
};
