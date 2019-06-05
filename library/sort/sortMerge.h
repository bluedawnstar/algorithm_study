#pragma once

template <typename T>
struct MergeSort {
    static void sort(T data[], int size) {
        if (size <= 1)
            return;

        unique_ptr<T> buff(new T[size]);
        sort(data, 0, size - 1, buff.get());
    }

    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }

private:
#if 0
    static void mergeInplace(T data[], int left, int mid, int right, T buff[]) {
        memcpy(buff, data + left, (right - left + 1) * sizeof(T));

        T* dest = data + left;

        T*  fst = buff;
        int fstN = mid - left + 1;
        T*  snd = buff + fstN;
        int sndN = right - mid;

        if (fstN > 0 && sndN > 0) {
            while (true) {
                if (*fst < *snd) {
                    *dest++ = *fst++;
                    if (--fstN <= 0)
                        break;
                } else {
                    *dest++ = *snd++;
                    if (--sndN <= 0)
                        break;
                }
            }
        }

        if (fstN > 0)
            memcpy(dest, fst, fstN * sizeof(T));
        else
            memcpy(dest, snd, sndN * sizeof(T));
    }
#else
    static void mergeInplace(T data[], int left, int mid, int right, T buff[]) {
        memcpy(buff, data + mid + 1, (right - mid) * sizeof(T));

        T* dest = data + right;

        T*  fst = data + mid;
        int fstN = mid - left + 1;

        int sndN = right - mid;
        T*  snd = buff + sndN - 1;

        if (fstN > 0 && sndN > 0) {
            while (true) {
                if (*snd < *fst) {
                    *dest-- = *fst--;
                    if (--fstN <= 0)
                        break;
                } else {
                    *dest-- = *snd--;
                    if (--sndN <= 0)
                        break;
                }
            }
        }

        if (sndN > 0)
            memcpy(data + left, buff, sndN * sizeof(T));
    }
#endif
    static void sort(T data[], int left, int right, T buff[]) {
        if (left < right) {
            int mid = (left + right) / 2;
            sort(data, left, mid, buff);
            sort(data, mid + 1, right, buff);
            mergeInplace(data, left, mid, right, buff);
        }
    }
};
