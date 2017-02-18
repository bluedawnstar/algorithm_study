## functions and classes for string manipulation

### Prefix function (prefixFunction.cpp)

`vector<int> prefixFunction(string s)`
 - KMP 알고리즘에서 사용하는 pattern의 partial matching 정보를 구한다.

`vector<int> prefixFunction(string s, int start)`
 - prefixFunction()과 동일하나 s의 start 부터의 suffix에 대해서만 계산한ㄴ다.

`pair<int,int> checkRepeatedString(const vector<int>& prefix, int len)`
 - input : prefix - prefixFunction() 실행 결과
 - input : len - 앞에서부터 len 길이까지에서 반복이 있는지 확인한다.
 - return : 반복 문자열이 아니면 (-1, -1)을, 반복 문자열이면 (반복 길이, 반복 개수)를 반환한다.
 - 문자열에 대한 prefixFunction()을 실행한 결과에서 원래 문자열 s의 앞에서부터 len 길이까지가 반복 문자열인지 확인한다.
 - 반복되는 패턴이 발견되면 (반복 길이, 반복 개수) 쌍을 반환한다.

`pair<int,int> checkRepeatedString(const vector<int>& prefix)`
 - input : prefix - prefixFunction() 실행 결과
 - return : 반복 문자열이 아니면 (-1, -1)을, 반복 문자열이면 (반복 길이, 반복 개수)를 반환한다.
 - 문자열에 대한 prefixFunction()을 실행한 결과에서 원래 문자열 s 전체가 반복 문자열인지 확인한다.
 - 반복되는 패턴이 발견되면 (반복 길이, 반복 개수) 쌍을 반환한다.

`pair<int, int> getMaxRepeatedPrefixString(const vector<int>& prefix)`
 - input : prefix - prefixFunction() 실행 결과
 - return : 반복 문자열이 없으면 (-1, -1)을, 있으면 가장긴 반복 문자열의 (반복 길이, 반복 개수)를 반환한다.
 - 문자열에 대한 prefixFunction()을 실행한 결과에서 앞에서 부터 반복 문자열로 구성된 최대 크기를 찾는다.
 - 반복되는 패턴이 발견되면 (반복 길이, 반복 개수) 쌍을 반환한다.

`pair<int, pair<int, int>> getMaxRepeatedSubstring(string s)`
 - input : s - 입력 문자열 
 - return : 반복 문자열이 없으면 (-1, (-1, -1))을, 있으면 가장긴 반복 문자열의 (위치, (반복 길이, 반복 개수))를 반환한다.
 - 문자열 s의 모든 substring 중에서 반복 문자열로 구성된 최대 크기를 찾는다.
 - 반복되는 패턴이 발견되면 (위치, (반복 길이, 반복 개수)) 쌍을 반환한다.

`vector<int> kmp(string s, string p)`
 - input : s - 입력 문자열 
 - input : p - 찾을 패턴
 - return : 패턴이 발견된 모든 위치를 반환한다.
 - KMP 알고리즘을 통해서 문자열 s에서 패턴 p의 모든 위치를 찾는다.

`vector<int> kmp(string s, string p, const vector<int>& prefix)`
 - input : s - 입력 문자열 
 - input : p - 찾을 패턴
 - input : prefix - p의 prefix function 실행 결과
 - return : 패턴이 발견된 모든 위치를 반환한다.
 - KMP 알고리즘을 통해서 문자열 s에서 패턴 p의 모든 위치를 찾는다.


### Suffix Array (suffixArray.cpp)

`template <typename T> vector<int> makeSuffixArray(T s, int n)`
 - 길이가 n인 문자열 s에서 suffix array를 생성한다.
 - 이 함수의 시간복잡도는 O(N (logN)^2) 이다.

`template <typename T> vector<int> makeSuffixArrayFast(T s, int n)`
 - 길이가 n인 문자열 s에서 suffix array를 생성한다.
 - 이 함수의 시간복잡도는 O(N logN)으로, 굉장히 빠른 속도를 동작한다.
   suffixArray_notUsed.cpp에 O(N) 알고리즘이 구현되어있지만, 실험결과 이 함수와 별반 성능 차이가 나지 않는다.
   
`template <typename T> int commonPrefix(T s, int n, int i, int j)`
 - 문자열 s에서 i번째 suffix와 j번째 suffix 사이의 LCP를 구한다.

`template <typename T> vector<int> getAllCommonPrefix(const vector<int>& suffixArray, T s, int n)`
 - 모든 suffix array 내용에 대한 LCP를 한꺼번에 구한다. (Kasai algorithm)
 - 시간 복잡도는 O(N)으로, 굉장히 빠르게 LCP를 구한다.

`long long countSubstrings(const vector<int>& suffixArray, T s, int n)`
 - Suffix array로부터 모든 substring의 개수를 구한다.
 - 시간 복잡도는 O(N^2)로, 별반 좋은 성능이 아니다.

`template <typename T> long long countSubstringsFast(const vector<int>& suffixArray, T s, int n)`
 - Suffix array로부터 모든 substring의 개수를 구한다.
 - 내부적으로 getAllCommonPrefix() 함수를 사용하며 전체 수행 시간이 O(N)이다.


### 문자열 자르기 (split.cpp)

`vector<string> split(string s, char c)`
 - Java나 다른 언어에는 문자열을 특정 delimitor로 구분해주는 기능들이 제공되는데, C++에는 없어서 유용하게 사용할 수 있다.
 - 기능은 매우 간단해서 문자열 s를 문자 c를 구분기호로 사용해서 잘라준다.

`vector<pair<int,int>> split2(string s, char c)`
 - split() 함수와 동일하나, 문자열 자체를 저장하는 것이 아니라 각 substring에 대해 s에서의 (위치, 길이)를 저정한다.
