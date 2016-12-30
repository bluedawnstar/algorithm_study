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

### 약수 관련 함수들 (factor.cpp)
약수와 관련된 함수들을 정의한다.
아래 일부 기능은 너무 간단해서 함수로 제공하지 않는 것들도 있다.

1. 약수 구하기

 `template <typename T> vector<T> getFactors(T x)`
  - x의 모든 약수를 구한다. O(sqrt(x))

 'vector<int> getFactors(const vector<pair<int, int>>& primeFactors)'
  - getPrimeFactors2()나 getPrimeFactors()로 구한 prime factor 정보를 이용해서 모든 약수를 생성한다.

2. 약수의 개수 구하기

 `int getFactorCount(const vector<pair<int, int>>& primeFactors)`
  - getPrimeFactors2()나 getPrimeFactors()로 구한 prime factor 정보를 이용해서 약수의 수를 계산한다.

 `vector<int> getAllFactorCount(const vector<int>& minFactors)`
  - getPrimeFactors()로 n 까지의 모든 정수에 대해 구한 prime factor를 이용해 n까지의 모든 약수의 개수를 구한다.
  - time complexity는 O(N) 정도 시간이 소요되나, 실제 실행해보면 아래 getAllFactorCountSimple()이 더 빠르다.

 `vector<int> getAllFactorCountSimple(int n)`
  - n까지의 모든 정수에 대해서 약수의 개수를 구한다.
  - 시간 복잡도는 O(N logN) 정도이나, 구현 내용이 매우 간단해서 실행은 getAllFactorCount()보다도 더 빠르다.

3. 약수 합 구하기
 - 약수의 합은 소인수 분해 결과를 이용하면 매우 빠르게 계산할 수 있다.
 - `A = a^α * b^β * c^γ` 일때, 약수의 전체 합은 아래와 같다.
   - `<=> (1 + a^1 + ... + a^α) * (1 + b^1 + ... + b^β) * (1 + c^1 + ... + c^γ)` 
   - `<=> (a^(α+1) - 1) / (a - 1) * (b^(β+1) - 1) / (b - 1) * (c^(γ+1) - 1) / (c - 1)`

4. 약수의 곱
 - 마찬가지로 소인수 분해 결과를 이용하면 매우 빠르게 계산할 수 있다.
 - `A = a^α * b^β * c^γ` 일때, 약수의 전체 곱은 아래와 같다.
   - `<=> A^((α + 1) * (β + 1) * (γ + 1) / 2)`
 - 즉, pow(A, 약수개수의_반절) 이 된다.

