#pragma once

/*
 
 <How to make persistent data structure for subtree query on a rooted tree>

    1. first DFS iteration to make basic information - O(N)

        int N;
        vector<int> edges[MAXN];

        int currTime = 0;
        pair<int, int> visitTime[MAXN];
        int timeToNode[MAXN];

        void dfs1(int u, int parent) {
            timeToNode[currTime] = u;
            visitTime[u].first = currTime++;
            for (auto v : edges[u]) {
                if (v == parent)
                    continue;
                dfs1(v, u);
            }
            visitTime[u].second = currTime;
        }
    
    2. second DFS iteration to build persistent data structures - O(NlogN)
        // smaller-to-larger optimization

        int nodeToRoot[MAXN];

        void dfs2(int u, int parent) {
            vector<pair<int,int>> subtrees;     // (subtree size of a node, node)

            for (auto v : edges[u]) {
                if (v == parent)
                    continue;
                dfs2(v, u);
                subtrees.emplace_back(visitTime[v].second - visitTime[v].first, v);
            }
            sort(subtrees.begin(), subtrees.end(), greater<pair<int,int>>());

            int root = -1;  //TODO: initial root
            if (!subtrees.empty()) {
                root = nodeToRoot[subtrees[0].second];
                for (int i = 1; i < int(subtrees.size()); i++) {
                    int tL = visitTime[subtrees[i].second].first;
                    int tR = visitTime[subtrees[i].second].second;
                    for (int t = tL; t < tR; t++)
                        root = persistentDS.add(root, timeToNode[t], ...);
                }
            }
            nodeToRoot[u] = persistentDS.add(root, u, ...);
        }

*/
