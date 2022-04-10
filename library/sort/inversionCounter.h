#pragma once


template <typename T>
struct InversionCounter {
    static long long count(const T data[], int size) {
        if (size <= 1)
            return 0ll;

        vector<T> copyed(data, data + size);
        unique_ptr<T> aux(new T[size]);
        return count(copyed.data(), 0, size - 1, aux.get());
    }

    static long long count(const vector<T>& data) {
        return count(data.data(), int(data.size()));
    }

private:
    static long long merge(T data[], int lo, int mid, int hi, T aux[]) {
        for (int i = lo, j = 0; i <= mid; i++, j++)
            aux[j] = data[i];

        long long res = 0;
        int cnt = 0;

        int i = mid + 1, j = 0, k = lo;
        while (i <= hi && j <= mid - lo) {
            if (aux[j] <= data[i]) {
                data[k++] = aux[j++];
                res += cnt;
            } else {
                data[k++] = data[i++];
                cnt++;
            }
        }

        while (j <= mid - lo) {
            data[k++] = aux[j++];
            res += cnt;
        }

        return res;
    }

    static long long count(T data[], int lo, int hi, T aux[]) {
        long long res = 0;
        if (lo < hi) {
            int mid = (lo + hi) / 2;
            res += count(data, lo, mid, aux);
            res += count(data, mid + 1, hi, aux);
            res += merge(data, lo, mid, hi, aux);
        }
        return res;
    }
};
