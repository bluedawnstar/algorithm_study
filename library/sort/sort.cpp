#include <memory>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "sortBubble.h"
#include "sortSelection.h"
#include "sortInsertion.h"
#include "sortShell.h"
#include "sortHeap.h"
#include "sortMerge.h"
#include "sortMergeEnhanced.h"
#include "sortMergeEnhanced2.h"
#include "sortMergeEnhanced3.h"
#include "sortMergeBottomUp.h"
#include "sortTim.h"
#include "sortQuick.h"
#include "sortQuick3way.h"
#include "sortIntro.h"

#include "partition.h"
#include "partition3way.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

template <typename T>
static bool checkSorted(const vector<T>& v) {
    for (int i = 1; i < int(v.size()); i++) {
        if (v[i - 1] > v[i])
            return false;
    }
    return true;
}

template <typename T>
static T getNth(vector<T> v, int nth) {
    sort(v.begin(), v.end());
    return v[nth];
}


#define CHECK(v)        do {                     \
                            if (!checkSorted(v)) \
                                cout << "ERROR at line " << __LINE__ << " : not sorted!" << endl; \
                        } while (false)

#define RUN1(method)    do {                \
                            auto v = vin;   \
                            method;         \
                            if (!checkSorted(v)) \
                                cout << "ERROR at line " << __LINE__ << " : not sorted!" << endl; \
                        } while (false)

#define RUN2(method, k) do {                \
                            auto v = vin;   \
                            method;         \
                            if (v[k] != getNth(vin, k)) \
                                cout << "ERROR at line " << __LINE__ << " : invalid value!" << endl; \
                        } while (false)

#define PROFILE(name, method)   \
                        do {    \
                            auto v = vin; \
                            PROFILE_START(name); \
                            for (int i = 0; i < T; i++) {   \
                                v = vin;                    \
                                method;                     \
                            }                               \
                            cout << "... ";                 \
                            PROFILE_STOP(name);             \
                            CHECK(v);                       \
                        } while (false)

#define SPEED(name, method)                         \
                        do {                        \
                            auto v = vin;           \
                            PROFILE_HI_START(name); \
                            method;                 \
                            cout << "... ";         \
                            PROFILE_HI_STOP(name);  \
                            CHECK(v);               \
                        } while (false)


void testSort() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- sort ---------------------------------" << endl;
    {
        int N = 10000;
        int T = 100;

#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif

        vector<int> vin(N);
        for (int i = 0; i < N; i++)
            vin[i] = RandInt32::get();

        for (int i = 0; i < T; i++) {
            RUN1(BubbleSort<int>::sort(v));
            RUN1(BubbleSort<int>::sort2(v));
            RUN1(SelectionSort<int>::sort(v));
            RUN1(InsertionSort<int>::sort(v));
            RUN1(ShellSort<int>::sort(v));
            RUN1(MaxHeap<int>::sort(v));
            RUN1(MergeSort<int>::sort(v));
            RUN1(MergeSortBottomUp<int>::sort(v));
            RUN1(MergeSortEnhanced<int>::sort(v));
            RUN1(MergeSortEnhanced2<int>::sort(v));
            RUN1(MergeSortEnhanced3<int>::sort(v));
            RUN1(quickSort(v, partitionHoare<int>));
            RUN1(quickSort(v, partitionHoare2<int>));
            RUN1(quickSort(v, partitionHoareWithMED3<int>));
            RUN1(quickSort(v, partitionLomuto<int>));
            RUN1(quickSort(v, partitionLomutoWithMED3<int>));
            RUN1(quickSort2(v, partitionHoare<int>));
            RUN1(quickSort2(v, partitionHoare2<int>));
            RUN1(quickSort2(v, partitionHoareWithMED3<int>));
            RUN1(quickSort2(v, partitionLomuto<int>));
            RUN1(quickSort2(v, partitionLomutoWithMED3<int>));
            RUN1(quickSort(v, partitionDijkstra3way<int>));
            RUN1(quickSort(v, partitionDijkstra3wayWithMED3<int>));
            RUN1(quickSort2(v, partitionDijkstra3way<int>));
            RUN1(quickSort2(v, partitionDijkstra3wayWithMED3<int>));
            RUN1(quickSort(v, partitionBentleyMcIlroy3way<int>));
            RUN1(quickSort2(v, partitionBentleyMcIlroy3way<int>));
            RUN1(quickSort3way(v, partitionDualPivot3way<int>));
            RUN1(quickSort3way2(v, partitionDualPivot3way<int>));
            RUN1(introSort(v, partitionHoare<int>));
            RUN1(introSort(v, partitionHoare2<int>));
            RUN1(introSort(v, partitionHoareWithMED3<int>));
            RUN1(TimSort<int>::sort(v));
        }
    }
    cout << "OK!" << endl;
    {
        int N = 10000;
        int T = 100;

#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif

        vector<int> vin(N);
        for (int i = 0; i < N; i++)
            vin[i] = RandInt32::get();

        for (int i = 0; i < T; i++) {
            for (int j = 0; j < N; j += N / 10) {
                RUN2(quickSelect(v, j, partitionHoare<int>), j);
                RUN2(quickSelect(v, j, partitionHoare2<int>), j);
                RUN2(quickSelect(v, j, partitionHoareWithMED3<int>), j);
                RUN2(quickSelect(v, j, partitionLomuto<int>), j);
                RUN2(quickSelect(v, j, partitionLomutoWithMED3<int>), j);
            }
        }
    }
    cout << "OK!" << endl;

    cout << "--- speed test #1 - randomized data" << endl;
    {
        int N = 100000;
        int T = 100;

#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif

        vector<int> vin(N);
        for (int i = 0; i < N; i++)
            vin[i] = RandInt32::get();

        //cout << "Insertion sort..." << endl;
        //PROFILE(insertion, InsertionSort<int>::sort(v));

        cout << "Shellsort..." << endl;
        PROFILE(shell, ShellSort<int>::sort(v));

        cout << "Heap sort..." << endl;
        PROFILE(heap, MaxHeap<int>::sort(v));
        cout << "Merge sort..." << endl;
        PROFILE(merge, MergeSort<int>::sort(v));
        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE(merge_bottom_up, MergeSortBottomUp<int>::sort(v));
        cout << "Merge sort (enhanced)..." << endl;
        PROFILE(merge_enhanced, MergeSortEnhanced<int>::sort(v));
        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE(merge_enhanced2, MergeSortEnhanced2<int>::sort(v));
        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE(merge_enhanced3, MergeSortEnhanced3<int>::sort(v));

        cout << "Quick sort with Hoare..." << endl;
        PROFILE(quick_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort with Hoare2..." << endl;
        PROFILE(quick_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort with Hoare MED3..." << endl;
        PROFILE(quick_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort with Lomuto ..." << endl;
        PROFILE(quick_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort with Lomuto MED3..." << endl;
        PROFILE(quick_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort2 with Hoare..." << endl;
        PROFILE(quick2_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort2 with Hoare2..." << endl;
        PROFILE(quick2_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort2 with Hoare MED3..." << endl;
        PROFILE(quick2_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort2 with Lomuto ..." << endl;
        PROFILE(quick2_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort2 with Lomuto MED3..." << endl;
        PROFILE(quick2_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort with Dijkstra 3-way..." << endl;
        PROFILE(quick_3way, quickSort(v, partitionDijkstra3way<int>));
        cout << "Quick sort with Dijkstra 3-way..." << endl;
        PROFILE(quick_3way_med3, quickSort(v, partitionDijkstra3wayWithMED3<int>));
        cout << "Quick sort2 with Dijkstra 3-way..." << endl;
        PROFILE(quick2_3way, quickSort2(v, partitionDijkstra3way<int>));
        cout << "Quick sort2 with Dijkstra 3-way..." << endl;
        PROFILE(quick2_3way_med3, quickSort2(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Quick sort with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick_bent_3way, quickSort(v, partitionBentleyMcIlroy3way<int>));
        cout << "Quick sort2 with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick2_bent_3way, quickSort2(v, partitionBentleyMcIlroy3way<int>));

        cout << "Quick sort with Dual Pivot 3-way..." << endl;
        PROFILE(quick_dual_pivot_3way, quickSort3way(v, partitionDualPivot3way<int>));
        cout << "Quick sort2 with Dual Pivot 3-way..." << endl;
        PROFILE(quick2_dual_pivot_3way, quickSort3way2(v, partitionDualPivot3way<int>));

        cout << "IntroSort with Hoare..." << endl;
        PROFILE(introsort_hoare, introSort(v, partitionHoare<int>));
        cout << "IntroSort with Hoare2..." << endl;
        PROFILE(introsort_hoare2, introSort(v, partitionHoare2<int>));
        cout << "IntroSort with Hoare MED3..." << endl;
        PROFILE(introsort_hoare_med3, introSort(v, partitionHoareWithMED3<int>));
        cout << "IntroSort with Lomuto ..." << endl;
        PROFILE(introsort_lomuto, introSort(v, partitionLomuto<int>));
        cout << "IntroSort with Lomuto MED3..." << endl;
        PROFILE(introsort_lomuto_med3, introSort(v, partitionLomutoWithMED3<int>));

        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way, introSort(v, partitionDijkstra3way<int>));
        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way_med3, introSort(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Tim sort..." << endl;
        PROFILE(tim, TimSort<int>::sort(v));
    }
    cout << "--- speed test #2 - 10x duplicated" << endl;
    {
        int N = 100000;
        int T = 100;

#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif

        vector<int> vin(N);
        for (int i = 0; i < N; i++)
            vin[i] = RandInt32::get() % (N / 10);

        //cout << "Insertion sort..." << endl;
        //PROFILE(insertion, InsertionSort<int>::sort(v));

        cout << "Shellsort..." << endl;
        PROFILE(shell, ShellSort<int>::sort(v));

        cout << "Heap sort..." << endl;
        PROFILE(heap, MaxHeap<int>::sort(v));
        cout << "Merge sort..." << endl;
        PROFILE(merge, MergeSort<int>::sort(v));
        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE(merge_bottom_up, MergeSortBottomUp<int>::sort(v));
        cout << "Merge sort (enhanced)..." << endl;
        PROFILE(merge_enhanced, MergeSortEnhanced<int>::sort(v));
        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE(merge_enhanced2, MergeSortEnhanced2<int>::sort(v));
        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE(merge_enhanced3, MergeSortEnhanced3<int>::sort(v));

        cout << "Quick sort with Hoare..." << endl;
        PROFILE(quick_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort with Hoare2..." << endl;
        PROFILE(quick_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort with Hoare MED3..." << endl;
        PROFILE(quick_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort with Lomuto ..." << endl;
        PROFILE(quick_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort with Lomuto MED3..." << endl;
        PROFILE(quick_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort2 with Hoare..." << endl;
        PROFILE(quick2_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort2 with Hoare2..." << endl;
        PROFILE(quick2_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort2 with Hoare MED3..." << endl;
        PROFILE(quick2_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort2 with Lomuto ..." << endl;
        PROFILE(quick2_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort2 with Lomuto MED3..." << endl;
        PROFILE(quick2_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort with 3-way..." << endl;
        PROFILE(quick_3way, quickSort(v, partitionDijkstra3way<int>));
        cout << "Quick sort with 3-way..." << endl;
        PROFILE(quick_3way_med3, quickSort(v, partitionDijkstra3wayWithMED3<int>));
        cout << "Quick sort2 with 3-way..." << endl;
        PROFILE(quick2_3way, quickSort2(v, partitionDijkstra3way<int>));
        cout << "Quick sort2 with 3-way..." << endl;
        PROFILE(quick2_3way_med3, quickSort2(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Quick sort with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick_bent_3way, quickSort(v, partitionBentleyMcIlroy3way<int>));
        cout << "Quick sort2 with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick2_bent_3way, quickSort2(v, partitionBentleyMcIlroy3way<int>));

        cout << "Quick sort with Dual Pivot 3-way..." << endl;
        PROFILE(quick_dual_pivot_3way, quickSort3way(v, partitionDualPivot3way<int>));
        cout << "Quick sort2 with Dual Pivot 3-way..." << endl;
        PROFILE(quick2_dual_pivot_3way, quickSort3way2(v, partitionDualPivot3way<int>));

        cout << "IntroSort with Hoare..." << endl;
        PROFILE(introsort_hoare, introSort(v, partitionHoare<int>));
        cout << "IntroSort with Hoare2..." << endl;
        PROFILE(introsort_hoare2, introSort(v, partitionHoare2<int>));
        cout << "IntroSort with Hoare MED3..." << endl;
        PROFILE(introsort_hoare_med3, introSort(v, partitionHoareWithMED3<int>));
        cout << "IntroSort with Lomuto ..." << endl;
        PROFILE(introsort_lomuto, introSort(v, partitionLomuto<int>));
        cout << "IntroSort with Lomuto MED3..." << endl;
        PROFILE(introsort_lomuto_med3, introSort(v, partitionLomutoWithMED3<int>));

        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way, introSort(v, partitionDijkstra3way<int>));
        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way_med3, introSort(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Tim sort..." << endl;
        PROFILE(tim, TimSort<int>::sort(v));
    }
    cout << "--- speed test #3 - 100x duplicated" << endl;
    {
        int N = 100000;
        int T = 100;

#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif

        vector<int> vin(N);
        for (int i = 0; i < N; i++)
            vin[i] = RandInt32::get() % (N / 100);

        //cout << "Insertion sort..." << endl;
        //PROFILE(insertion, InsertionSort<int>::sort(v));

        cout << "Shellsort..." << endl;
        PROFILE(shell, ShellSort<int>::sort(v));

        cout << "Heap sort..." << endl;
        PROFILE(heap, MaxHeap<int>::sort(v));
        cout << "Merge sort..." << endl;
        PROFILE(merge, MergeSort<int>::sort(v));
        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE(merge_bottom_up, MergeSortBottomUp<int>::sort(v));
        cout << "Merge sort (enhanced)..." << endl;
        PROFILE(merge_enhanced, MergeSortEnhanced<int>::sort(v));
        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE(merge_enhanced2, MergeSortEnhanced2<int>::sort(v));
        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE(merge_enhanced3, MergeSortEnhanced3<int>::sort(v));

        cout << "Quick sort with Hoare..." << endl;
        PROFILE(quick_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort with Hoare2..." << endl;
        PROFILE(quick_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort with Hoare MED3..." << endl;
        PROFILE(quick_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort with Lomuto ..." << endl;
        PROFILE(quick_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort with Lomuto MED3..." << endl;
        PROFILE(quick_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort2 with Hoare..." << endl;
        PROFILE(quick2_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort2 with Hoare2..." << endl;
        PROFILE(quick2_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort2 with Hoare MED3..." << endl;
        PROFILE(quick2_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort2 with Lomuto ..." << endl;
        PROFILE(quick2_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort2 with Lomuto MED3..." << endl;
        PROFILE(quick2_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort with 3-way..." << endl;
        PROFILE(quick_3way, quickSort(v, partitionDijkstra3way<int>));
        cout << "Quick sort with 3-way..." << endl;
        PROFILE(quick_3way_med3, quickSort(v, partitionDijkstra3wayWithMED3<int>));
        cout << "Quick sort2 with 3-way..." << endl;
        PROFILE(quick2_3way, quickSort2(v, partitionDijkstra3way<int>));
        cout << "Quick sort2 with 3-way..." << endl;
        PROFILE(quick2_3way_med3, quickSort2(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Quick sort with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick_bent_3way, quickSort(v, partitionBentleyMcIlroy3way<int>));
        cout << "Quick sort2 with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick2_bent_3way, quickSort2(v, partitionBentleyMcIlroy3way<int>));

        cout << "Quick sort with Dual Pivot 3-way..." << endl;
        PROFILE(quick_dual_pivot_3way, quickSort3way(v, partitionDualPivot3way<int>));
        cout << "Quick sort2 with Dual Pivot 3-way..." << endl;
        PROFILE(quick2_dual_pivot_3way, quickSort3way2(v, partitionDualPivot3way<int>));

        cout << "IntroSort with Hoare..." << endl;
        PROFILE(introsort_hoare, introSort(v, partitionHoare<int>));
        cout << "IntroSort with Hoare2..." << endl;
        PROFILE(introsort_hoare2, introSort(v, partitionHoare2<int>));
        cout << "IntroSort with Hoare MED3..." << endl;
        PROFILE(introsort_hoare_med3, introSort(v, partitionHoareWithMED3<int>));
        cout << "IntroSort with Lomuto ..." << endl;
        PROFILE(introsort_lomuto, introSort(v, partitionLomuto<int>));
        cout << "IntroSort with Lomuto MED3..." << endl;
        PROFILE(introsort_lomuto_med3, introSort(v, partitionLomutoWithMED3<int>));

        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way, introSort(v, partitionDijkstra3way<int>));
        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way_med3, introSort(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Tim sort..." << endl;
        PROFILE(tim, TimSort<int>::sort(v));
    }
    cout << "OK!" << endl;

    cout << "--- speed test #4 - almost sorted" << endl;
    {
        int N = 100000;
        int T = 100;

#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif

        vector<int> vin(N);
        for (int i = 0; i < N; i++)
            vin[i] = i;

        for (int i = 0; i < N / 100; i++) {
            int a = RandInt32::get() % N;
            int b = RandInt32::get() % N;
            swap(vin[a], vin[b]);
        }

        //cout << "Insertion sort..." << endl;
        //PROFILE(insertion, InsertionSort<int>::sort(v));

        cout << "Shellsort..." << endl;
        PROFILE(shell, ShellSort<int>::sort(v));

        cout << "Heap sort..." << endl;
        PROFILE(heap, MaxHeap<int>::sort(v));
        cout << "Merge sort..." << endl;
        PROFILE(merge, MergeSort<int>::sort(v));
        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE(merge_bottom_up, MergeSortBottomUp<int>::sort(v));
        cout << "Merge sort (enhanced)..." << endl;
        PROFILE(merge_enhanced, MergeSortEnhanced<int>::sort(v));
        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE(merge_enhanced2, MergeSortEnhanced2<int>::sort(v));
        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE(merge_enhanced3, MergeSortEnhanced3<int>::sort(v));

        cout << "Quick sort with Hoare..." << endl;
        PROFILE(quick_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort with Hoare2..." << endl;
        PROFILE(quick_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort with Hoare MED3..." << endl;
        PROFILE(quick_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort with Lomuto ..." << endl;
        PROFILE(quick_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort with Lomuto MED3..." << endl;
        PROFILE(quick_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort2 with Hoare..." << endl;
        PROFILE(quick2_hoare, quickSort(v, partitionHoare<int>));
        cout << "Quick sort2 with Hoare2..." << endl;
        PROFILE(quick2_hoare2, quickSort(v, partitionHoare2<int>));
        cout << "Quick sort2 with Hoare MED3..." << endl;
        PROFILE(quick2_hoare_med3, quickSort(v, partitionHoareWithMED3<int>));
        cout << "Quick sort2 with Lomuto ..." << endl;
        PROFILE(quick2_lomuto, quickSort(v, partitionLomuto<int>));
        cout << "Quick sort2 with Lomuto MED3..." << endl;
        PROFILE(quick2_lomuto_med3, quickSort(v, partitionLomutoWithMED3<int>));

        cout << "Quick sort with 3-way..." << endl;
        PROFILE(quick_3way, quickSort(v, partitionDijkstra3way<int>));
        cout << "Quick sort with 3-way..." << endl;
        PROFILE(quick_3way_med3, quickSort(v, partitionDijkstra3wayWithMED3<int>));
        cout << "Quick sort2 with 3-way..." << endl;
        PROFILE(quick2_3way, quickSort2(v, partitionDijkstra3way<int>));
        cout << "Quick sort2 with 3-way..." << endl;
        PROFILE(quick2_3way_med3, quickSort2(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Quick sort with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick_bent_3way, quickSort(v, partitionBentleyMcIlroy3way<int>));
        cout << "Quick sort2 with Bentley-McIlroy 3-way..." << endl;
        PROFILE(quick2_bent_3way, quickSort2(v, partitionBentleyMcIlroy3way<int>));

        cout << "Quick sort with Dual Pivot 3-way..." << endl;
        PROFILE(quick_dual_pivot_3way, quickSort3way(v, partitionDualPivot3way<int>));
        cout << "Quick sort2 with Dual Pivot 3-way..." << endl;
        PROFILE(quick2_dual_pivot_3way, quickSort3way2(v, partitionDualPivot3way<int>));

        cout << "IntroSort with Hoare..." << endl;
        PROFILE(introsort_hoare, introSort(v, partitionHoare<int>));
        cout << "IntroSort with Hoare2..." << endl;
        PROFILE(introsort_hoare2, introSort(v, partitionHoare2<int>));
        cout << "IntroSort with Hoare MED3..." << endl;
        PROFILE(introsort_hoare_med3, introSort(v, partitionHoareWithMED3<int>));
        cout << "IntroSort with Lomuto ..." << endl;
        PROFILE(introsort_lomuto, introSort(v, partitionLomuto<int>));
        cout << "IntroSort with Lomuto MED3..." << endl;
        PROFILE(introsort_lomuto_med3, introSort(v, partitionLomutoWithMED3<int>));

        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way, introSort(v, partitionDijkstra3way<int>));
        cout << "IntroSort with 3-way..." << endl;
        PROFILE(introsort_3way_med3, introSort(v, partitionDijkstra3wayWithMED3<int>));

        cout << "Tim sort..." << endl;
        PROFILE(tim, TimSort<int>::sort(v));
    }
    cout << "OK!" << endl;

//#ifndef _DEBUG
#if 0
    cout << "--- speed test per N " << endl;
    {
        //for (int N = 10; N <= 10'000'000; N *= 10) {
        int N = 10'000'000;
        vector<int> vin(N);
        for (int i = 0; i < N; i++)
            vin[i] = RandInt32::get();

        cout << "-- N = " << N << endl;

        //SPEED(insertion, InsertionSort<int>::sort(v));
        SPEED(shell, ShellSort<int>::sort(v));
        SPEED(heap, MaxHeap<int>::sort(v));
        SPEED(merge, MergeSort<int>::sort(v));
        SPEED(merge_bottom_up, MergeSortBottomUp<int>::sort(v));
        SPEED(merge_enhanced, MergeSortEnhanced<int>::sort(v));
        SPEED(merge_enhanced2, MergeSortEnhanced2<int>::sort(v));
        SPEED(merge_enhanced3, MergeSortEnhanced3<int>::sort(v));
        SPEED(quick, quickSort(v, partitionHoare2<int>));
        SPEED(quick_lomuto, quickSort(v, partitionLomuto<int>));
        SPEED(tim, TimSort<int>::sort(v));
    }
#endif
}
