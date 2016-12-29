## Sorting

### Query Square Root Decomposition 관련 함수 (sortSqrt.cpp)
주어진 데이터에 대해서 Left와 Right 범위 쌍(L, R)을 query로 주는 경우,
이 입력의 개수가 많아질 때 효과적으로 문제를 풀 수 있도록 하는 알고리즘이 개발되어있다.
MO's algorithm이라고 하는 것인데, query 쌍의 시작 위치가 sqrt(N) 블록 안에 들어오는 것들을
정렬해서 묶은 다음 query 간의 변화 부분만 업데이트 함으로서 시간 복잡도를 O((N + Q) * logN) 시간으로
줄일 수 있는 방법이다.
이를 위해서 query를 정렬해야하는데, 그 방법을 함수로 구현한 것이다.

`template <typename T> void sortSqrt(vector<pair<T, T>>& vec, int blockSize)`
 - pair<T,T> 쌍으로 되어있는 vector를 정렬한다.

`template <typename T, typename U> void sortSqrt(vector<pair<T, T>, U>& vec, int blockSize)`
 - pair<pair<T,T>, U> 쌍으로 되어있는 vector를 정렬한다. (L, R) 쌍과 그와 관련된 데이터가 있을 때 사용하면 된다.
