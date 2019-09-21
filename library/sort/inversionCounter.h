#pragma once


template <typename T>
struct InversionCounter {
    static long long count(const T data[], int size) {
        if (size <= 1)
            return 0ll;

        vector<T> copyed(data, data + size);
        unique_ptr<T> buff(new T[size]);
        return count(copyed.data(), 0, size - 1, buff.get());
    }

    static long long count(const vector<T>& data) {
        return count(data.data(), int(data.size()));
    }

private:
    static long long mergeInplace(T data[], int left, int mid, int right, T buff[]) {
        memcpy(buff, data + mid + 1, (right - mid) * sizeof(T));

        T* dest = data + right;

        T*  fst = data + mid;
        int fstN = mid - left + 1;

        int sndN = right - mid;
        T*  snd = buff + sndN - 1;

        long long res = 0;

        int cnt = 0;
        if (fstN > 0 && sndN > 0) {
            while (true) {
                if (*snd < *fst) {
                    *dest-- = *fst--;
                    ++cnt;
                    if (--fstN <= 0)
                        break;
                } else {
                    *dest-- = *snd--;
                    res += cnt;
                    if (--sndN <= 0)
                        break;
                }
            }
        }

        if (sndN > 0) {
            memcpy(data + left, buff, sndN * sizeof(T));
            res += 1ll * cnt * sndN;
        }

        return res;
    }

    static long long count(T data[], int left, int right, T buff[]) {
        long long res = 0;
        if (left < right) {
            int mid = (left + right) / 2;
            res += count(data, left, mid, buff);
            res += count(data, mid + 1, right, buff);
            res += mergeInplace(data, left, mid, right, buff);
        }
        return res;
    }
};
