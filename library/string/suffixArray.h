#pragma once

//--------- Building Suffix Array ---------------------------------------------

//--- O(N (logN)^2) method ---
template <typename T>
vector<int> makeSuffixArray(T s, int n);

//--- O(N logN) ---
template <typename T>
vector<int> makeSuffixArrayFast(T s, int n);

//--------- Calculating LCP ---------------------------------------------------

// O(N)
template <typename T>
int commonPrefix(T s, int n, int i, int j);

// Kasai algorithm - O(N)
template <typename T>
vector<int> makeLcpArray(const vector<int>& suffixArray, T s, int n);
