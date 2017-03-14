#pragma once

bool isPrimeNumber(int x);

vector<int> findPrimeNumbers(int n);

vector<int> getPrimeFactors(int x);
vector<pair<int, int>> getPrimeFactors2(int x);
void getPrimeFactors(int n, vector<int>& primes, vector<vector<pair<int, int>>>& primeFactors);

vector<bool> eratosthenes(int n);

// min prime factors
vector<int> getMinFactors(int n);
bool isPrimeNumber(const vector<int>& minFactors, int x);
vector<int> getPrimeFactors(const vector<int>& minFactors, int x);
vector<pair<int, int>> getPrimeFactors2(const vector<int>& minFactors, int x);
