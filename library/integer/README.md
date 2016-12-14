## Number Theory

### 소수관련 기본 함수 (primeNumberBasic.cpp)

`bool isPrimeNumber(int n)`
 - n이 소수인지 확인

`vector<int> getPrimeFactors(n)`
 - n에 대한 소인수 분해함 (예: 12 = (2,2,3))

`vector<pair<int, int>> getPrimeFactors2(int n)`
 - n에 대한 소인수 분해 결과를 구함 (예: 12 = ((2, 2), (3, 1))
 - 결과는 소수(first)와 그 소수의 멱승(second)임 

`vector<int> findPrimeNumbers(int n)`
 - 1에서 n까지의 정수 중 소수를 모두 찾는다.

`void getPrimeFactors(int n, vector<int>& primes,
                      vector<vector<pair<int, int>>>& primeFactors)`
 - 1에서 n까지의 정수 중 모든 소수를 찾고, 각 소수의 소인수분해 결과를 함께 구한다.

### 소수관련 개선 함수 (primeNumberEratosthenes.cpp)
범위 내의 소수를 구하는 방법으로 가장 성능이 좋은 알고리즘이 에라토스테네스의 체(Sieve of Eratosthenes)가 있다.
원리는 아주 간단하다. 소수를 발견하면 그 소수의 배수에 있는 모든 수들을 소수에서 지우는 것이다. 범위 내의 숫자들 중 소수가 아닌 것들을 체로 거르는 과정을 반복하다보면 결국 소수만 남게된다.

1. 범위 내의 모든 정수에 대해 소수 여부 판별 - 에라토스테네스의 체
 `vector<bool> eratosthenes(int n)`
  - 에리토스테네스의 체를 통해 소수판정을 한다.

2. 에라토스테네스의 체를 이용한 소인수분해
 - 에라토스테네스의 체를 바탕으로 보다 효과적인 소인수 분해가 가능하다.
 - 또한 아래 함수들은 특정 범위내의 모든 수들에 대한 소인수 분해 결과를 미리 모두 구하는 것이 아니라,
   소인수 중에서 가장 작은 값만을 가지고 있다가(getMinFactors) 필요할 때 직접 소인수 분해(getPrimeFactors)를 수행하는 함수다.

 `vector<int> getMinFactors(int n)`
  - 각 정수에 대해 가장 작은 소인수를 구한다.
  - 이 함수의 결과를 가지고 나머지 함수들을 실행해야 한다.

 `bool isPrimeNumber(vector<int>& minFactors, int x)`
  - 소수 여부 확인한다.

 `vector<int> getPrimeFactors(vector<int>& minFactors, int x)`
  - 모든 소인수를 구한다. (예: 12 = (2, 2, 3))

 `vector<pair<int, int>> getPrimeFactors2(vector<int>& minFactors, int x)`
  - 소인수와 소인수의 멱승을 구한다. (예: 12 = ((2,2), (3, 1)))
