#pragma once

template <typename T>
bool nextPermutation(vector<T>& v) {
    int N = int(v.size());
    for (int i = N - 1; i > 0; i--) {
        if (v[i - 1] < v[i]) {
            int next = i;
            for (int j = N - 1; j > i; j--) {
                if (v[i - 1] < v[j]) {
                    next = j;
                    break;
                }
            }
            swap(v[i - 1], v[next]);
            reverse(v.begin() + i, v.end());
            return true;
        }
    }

    return false;
}

template <typename T>
bool nextPermutation(T* v, int n) {
    for (int i = n - 1; i > 0; i--) {
        if (v[i - 1] < v[i]) {
            int next = i;
            for (int j = n - 1; j > i; j--) {
                if (v[i - 1] < v[j]) {
                    next = j;
                    break;
                }
            }
            swap(v[i - 1], v[next]);
            reverse(v + i, v + n);
            return true;
        }
    }

    return false;
}
