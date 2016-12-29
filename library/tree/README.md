## Tree

### 기본적인 Tree 관련 함수 (treeBasic.cpp)
Tree 관련 기본적인 함수들과 틀을 구현한 파일이다.

`void clear()`
 - loop을 돌면서 tree를 여러번 생성할 때 이전 데이터를 삭제하는 함수

`void dfs(int u, int parent)`
 - DFS로 tree를 순회하면서 각 노드의 parent 및 level table을 채운다.

`void dfsIter(int root)`
 - dfs()는 stack을 이용해서 동작하기 때문에 노드 수가 많아지면 stack overflow 가능성이 있다.
 - dfsIter()는 iterative 방법으로 동작하기 때문에 stack overflow 걱정없이 사용할 수 있다.
 - 단, dfs()보다 약 5배 정도 느리므로 N이 stack 크기를 넘지 않는다고 확신할 경우는 dfs()를 호출하는 것이 좋다.

`void bfs(int root)`
 - BFS로 tree를 순회하면서 각 노드의 parent 및 level table을 채운다.

`void makeLcaTable()`
 - LCA를 사용하기 위한 테이블을 초기화 한다.

`int climbTree(int node, int dist)`
 - node로부터 root 방향으로 dist 거리만큼 떨어진 조상 노드 번호를 반환한다.
 - makeLcaTable()로 LCA 테이블을 초기화 한 하에 사용 가능하다.

`int findLCA(int A, int B)`
 - A와 B 노드의 LCA를 구한다.
 - makeLcaTable()로 LCA 테이블을 초기화 한 하에 사용 가능하다.

### Tree에 MO's algorithm을 적용하기 위한 코드 템플릿 (treeMO.cpp)
자체로 동작하는 코드는 아니고, tree를 구성하고 MO's algorithm을 실행하기 위한 코드 템플릿이다.
이 코드들을 사용하기 위해서는 복사한 후 "TODO" 부분을 수정해야 한다.

'void initTreeMO(vector<pair<int, int>>& Q, vector<int>& lca, vector<pair<pair<int, int>, int>>& MO)'
 - MO 알고리즘을 구동하기 위해서 Q로부터 lca와 MO 리스트를 초기화 하는 함수다.
 - 이 함수를 호출하기 전에 Q에 (L, R) 쌍이 채워져있어야 하고, `L's visit time <= R's visit time` 이어야 한다.

`void addMO(int t)`
 - Add 기능을 구현한다.

`void removeMO(int t)`
 - Remove 기능을 구현한다.

`void testTreeMO()`
 - main() 함수에 해당하는 함수이며, 이 함수의 내용을 main() 함수에 적용하면 된다.
