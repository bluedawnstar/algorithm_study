#pragma once

template <typename T>
vector<T> getFactors(T x);
vector<int> getFactors(const vector<pair<int, int>>& primeFactors);

int getFactorCount(const vector<pair<int, int>>& primeFactors);
vector<int> getAllFactorCount(const vector<int>& minFactors);
vector<int> getAllFactorCountSimple(int n);
