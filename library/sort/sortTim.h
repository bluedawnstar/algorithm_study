#pragma once

/*
    TimSort, 1993
    - stable sort
    - time complexity : O(nlogn)
    - extra space : n/2 -> but n in this implementation

    https://en.wikipedia.org/wiki/Timsort
      - "Timsort is a hybrid stable sorting algorithm, derived from merge sort and insertion sort,
         designed to perform well on many kinds of real-world data."
      - "It was implemented by Tim Peters in 2002 for use in the Python programming language."
        "The algorithm finds subsequences of the data that are already ordered (runs)
         and uses them to sort the remainder more efficiently."
        "This is done by merging runs until certain criteria are fulfilled."
      - "Timsort has been Python's standard sorting algorithm since version 2.3."
        "It is also used to sort arrays of non-primitive type
          - in Java SE 7,
          - on the Android platform,
          - in GNU Octave,
          - on V8,
          - Swift,
          - and Rust."
      - "It uses techniques from Peter McIlroy's 1993 paper "Optimistic Sorting and Information Theoretic Complexity"."

    https://bugs.python.org/file4451/timsort.txt
*/
template <typename T>
class TimSort {
    static const int MIN_MERGE = 32;
    static const int MIN_GALLOP = 7;

public:
    static void sort(T a[], int n) {
        unique_ptr<T> aux(new T[n]);
        vector<pair<int, int>> stack;   // { (base, length), ... }
        stack.reserve(40);

        int lo = 0;
        do {
            int runLen = countRunAndMakeAscending(a, lo, n);
            if (runLen < MIN_MERGE) {
                int len = min(MIN_MERGE, n - lo);
                binaryInsertionSort(a, lo, lo + len, lo + runLen);
                runLen = len;
            }
            pushRun(a, lo, runLen, aux.get(), stack);

            lo += runLen;
        } while (lo < n);

        mergeAllRuns(a, aux.get(), stack);
    }

    static void sort(vector<T>& a) {
        sort(a.data(), int(a.size()));
    }

private:
    static void copy(const T src[], int srcFirst, T dst[], int dstFirst, int size) {
        //while (size-- > 0)
        //    dst[dstFirst++] = src[srcFirst++];
        memcpy(dst + dstFirst, src + srcFirst, sizeof(T) * size);
    }

    static void copyAndAdvance(const T src[], int& srcFirst, T dst[], int& dstFirst, int size) {
        if (size > 0) {
            copy(src, srcFirst, dst, dstFirst, size);
            srcFirst += size;
            dstFirst += size;
        }
    }

    static void copyBackward(const T src[], int srcLast, T dst[], int dstLast, int size) {
        //while (size-- > 0)
        //    dst[dstLast--] = src[srcLast--];
        memmove(dst + dstLast - size + 1, src + srcLast - size + 1, sizeof(T) * size);
    }

    static void copyBackwardAndAdvance(const T src[], int& srcLast, T dst[], int& dstLast, int size) {
        if (size > 0) {
            copyBackward(src, srcLast, dst, dstLast, size);
            srcLast -= size;
            dstLast -= size;
        }
    }

    //--- binary insertion sort

    // compare = O(n*logn), copy = O(n^2)
    static void binaryInsertionSort(T a[], int first, int last, int start) {
        if (start == first)
            start++;
        for (; start < last; start++) {
            T pivot = a[start];

            // binary search
            int lo = first;
            int hi = start - 1;
            while (lo <= hi) {
                int mid = (lo + hi) >> 1;
                if (pivot < a[mid])
                    hi = mid - 1;
                else
                    lo = mid + 1;
            }

            // rotate right
            copyBackward(a, start - 1, a, start, start - lo);
            a[lo] = pivot;
        }
    }

    // https://en.wikipedia.org/wiki/Exponential_search
    static int lowerBound(T key, T a[], int first, int last, bool forward) {
        int hint = forward ? first : last - 1;

        int lo, hi;

        // gallop hi/lo until a[base + lo] < key <= a[base + hi]
        if (key > a[hint]) {
            int size = 1;
            int maxSize = last - hint;
            while (size < maxSize && key > a[hint + size])
                size <<= 1;

            lo = hint + (size >> 1);
            hi = min(hint + size, last) - 1;
        } else {
            int size = 1;
            int maxSize = hint - first + 1;
            while (size < maxSize && key <= a[hint - size])
                size <<= 1;

            lo = max(first, hint - size);
            hi = hint - (size >> 1) - 1;
        }

        // binary search
        while (lo <= hi) {
            int mid = lo + ((hi - lo) >> 1);
            if (key > a[mid])
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        return lo;
    }

    // https://en.wikipedia.org/wiki/Exponential_search
    static int upperBound(T key, T a[], int first, int last, bool forward) {
        int hint = forward ? first : last - 1;

        int lo, hi;

        // gallop lo/hi until a[base + lo] <= key < a[b + hi]
        if (key >= a[hint]) {
            int size = 1;
            int maxSize = last - hint;
            while (size < maxSize && key >= a[hint + size])
                size <<= 1;

            lo = hint + (size >> 1);
            hi = min(hint + size, last) - 1;
        } else {
            int size = 1;
            int maxSize = hint - first + 1;
            while (size < maxSize && key < a[hint - size])
                size <<= 1;

            lo = max(first, hint - size);
            hi = hint - (size >> 1) - 1;
        }

        // binary search
        while (lo <= hi) {
            int mid = lo + ((hi - lo) >> 1);
            if (key >= a[mid])
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        return lo;
    }

    static int countRunAndMakeAscending(T a[], int first, int last) {
        int i = first + 1;
        if (i == last)
            return 1;

        if (a[i++] < a[first]) {    // descending
            while (i < last && a[i] < a[i - 1])
                i++;
            reverse(a + first, a + i);
        } else {                    // ascending
            while (i < last && a[i] >= a[i - 1])
                i++;
        }
        return i - first;
    }

    //--- merge operations

    // <precondition>
    //   - a[first1] > a[first2]
    //   - a[first2 - 1] > a[last - 1]
    static void mergeL2R(T a[], int first1, int first2, int last, T aux[]) {
        copy(a, first1, aux, first1, first2 - first1);

        int i = first2, j = first1, k = first1;

        a[k++] = a[i++];
        while (i < last && j + 1 < first2) {
            // normal merge
            int cnt1 = 0, cnt2 = 0;
            do {
                if (a[i] < aux[j]) {
                    a[k++] = a[i++];
                    cnt2++;
                    cnt1 = 0;
                } else {
                    a[k++] = aux[j++];
                    cnt1++;
                    cnt2 = 0;
                }
            } while (j + 1 < first2 && i < last && (cnt1 | cnt2) < MIN_GALLOP);

            // galloping mode
            while (j + 1 < first2 && i < last && (cnt1 >= MIN_GALLOP || cnt2 >= MIN_GALLOP)) {
                cnt1 = upperBound(a[i], aux, j, first2, true) - j;
                copyAndAdvance(aux, j, a, k, cnt1);
                a[k++] = a[i++];

                //if (i < last)
                {
                    cnt2 = lowerBound(aux[j], a, i, last, true) - i;
                    copyAndAdvance(a, i, a, k, cnt2);
                    a[k++] = aux[j++];
                }
            }
        }

        if (j + 1 == first2) {
            copyAndAdvance(a, i, a, k, last - i);
            a[k] = aux[j];
        } else if (i == last) {
            copyAndAdvance(aux, j, a, k, first2 - j);
        }
    }

    // <precondition>
    //   - a[first1] > a[first2]
    //   - a[first2 - 1] > a[last - 1]
    static void mergeR2L(T a[], int first1, int first2, int last, T aux[]) {
        copy(a, first2, aux, first2, last - first2);

        int i = first2 - 1;
        int j = last - 1;
        int k = last - 1;

        a[k--] = a[i--];
        while (i >= first1 && j > first2) {
            // normal merge
            int cnt1 = 0, cnt2 = 0;
            do {
                if (aux[j] < a[i]) {
                    a[k--] = a[i--];
                    cnt1++;
                    cnt2 = 0;
                } else {
                    a[k--] = aux[j--];
                    cnt2++;
                    cnt1 = 0;
                }
            } while (i >= first1 && j > first2 && (cnt1 | cnt2) < MIN_GALLOP);

            // galloping mode
            while (i >= first1 && j > first2 && (cnt1 >= MIN_GALLOP || cnt2 >= MIN_GALLOP)) {
                cnt1 = (i + 1) - upperBound(aux[j], a, first1, i + 1, false);
                copyBackwardAndAdvance(a, i, a, k, cnt1);
                a[k--] = aux[j--];

                if (i >= first1) {
                    cnt2 = (j + 1) - lowerBound(a[i], aux, first2, j + 1, false);
                    copyBackwardAndAdvance(aux, j, a, k, cnt2);
                    a[k--] = a[i--];
                }
            }
        }

        if (j == first2) {
            copyBackwardAndAdvance(a, i, a, k, i - first1 + 1);
            a[first1] = aux[j];
        } else if (i < first1) {
            copyBackwardAndAdvance(aux, j, a, k, j - first2 + 1);
        }
    }

    static void merge(T a[], int first1, int first2, int last, T aux[]) {
        // run1에서 skip해도 되는 앞부분 계산
        int k = upperBound(a[first2], a, first1, first2, true) - first1;
        first1 += k;
        if (first1 >= first2)
            return;

        // run2에서 그대로 있어도 되는 뒷부분 계산
        k = lowerBound(a[first2 - 1], a, first2, last, false) - first2;
        if (k == 0)
            return;
        last = first2 + k;

        if (first2 - first1 <= last - first2)
            mergeL2R(a, first1, first2, last, aux);
        else
            mergeR2L(a, first1, first2, last, aux);
    }

    //--- run stack

    static void pushRun(T a[], int base, int len, T aux[], vector<pair<int, int>>& stack) {
        int i;
        for (i = 0; i < int(stack.size()) && stack[i].second > 0; i++) {
            merge(a, stack[i].first, base, base + len, aux);
            base = stack[i].first;
            len += stack[i].second;
            stack[i] = make_pair(0, 0);
        }

        if (i < int(stack.size()))
            stack[i] = make_pair(base, len);
        else
            stack.emplace_back(base, len);
    }

    static void mergeAllRuns(T a[], T aux[], vector<pair<int, int>>& stack) {
        int base = 0, len = 0;
        for (int i = 0; i < int(stack.size()); i++) {
            if (stack[i].second > 0) {
                if (len == 0) {
                    base = stack[i].first;
                    len = stack[i].second;
                } else {
                    merge(a, stack[i].first, base, base + len, aux);
                    base = stack[i].first;
                    len += stack[i].second;
                }
            }
        }
    }
};
