#include <memory>
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
#include "sortQuick.h"
#include "sortQuick3way.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"


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
            auto vout00 = vin;
            BubbleSort<int>::sort(vout00);

            auto vout01 = vin;
            BubbleSort<int>::sort2(vout01);

            auto voutSel = vin;
            SelectionSort<int>::sort(voutSel);

            auto voutIns = vin;
            InsertionSort<int>::sort(voutIns);

            auto voutShell = vin;
            ShellSort<int>::sort(voutShell);

            auto vout1 = vin;
            MaxHeap<int>::sort(vout1);

            auto vout2 = vin;
            MergeSort<int>::sort(vout2);
            auto vout2_1 = vin;
            MergeSortBottomUp<int>::sort(vout2_1);
            auto vout2_2 = vin;
            MergeSortEnhanced<int>::sort(vout2_2);
            auto vout2_3 = vin;
            MergeSortEnhanced2<int>::sort(vout2_3);
            auto vout2_4 = vin;
            MergeSortEnhanced3<int>::sort(vout2_4);

            auto vout3 = vin;
            QuickSort<int>::sort(vout3);

            auto vout3_3 = vin;
            QuickSort<int>::sort(vout3_3);

            auto vout4 = vin;
            QuickSort3way<int>::sort(vout4);

            auto vout4_2 = vin;
            QuickSort3way<int>::sort2(vout4_2);

            if (vout00 != vout1 || vout01 != vout1
                || voutSel != vout1
                || voutIns != vout1
                || voutShell != vout1
                || vout2 != vout1
                || vout2_1 != vout1
                || vout2_2 != vout1
                || vout2_3 != vout1
                || vout2_4 != vout1
                || vout3 != vout1 || vout3_3 != vout1
                || vout4 != vout1 || vout4_2 != vout1)
                cout << "ERROR : mismatched!" << endl;

            assert(vout00 == vout1);
            assert(vout01 == vout1);
            assert(voutSel == vout1);
            assert(voutIns == vout1);
            assert(voutShell == vout1);
            assert(vout2 == vout1);
            assert(vout2_1 == vout1);
            assert(vout2_2 == vout1);
            assert(vout2_3 == vout1);
            assert(vout2_4 == vout1);
            assert(vout3 == vout1);
            assert(vout3_3 == vout1);
            assert(vout4 == vout1);
            assert(vout4_2 == vout1);
        }
    }
    cout << "OK!" << endl;

    cout << "--- speed test #1" << endl;
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

        /*
        cout << "Bubble sort #1..." << endl;
        PROFILE_START(bubble_1);
        auto vout10 = vin;
        for (int i = 0; i < T; i++) {
            vout10 = vin;
            BubbleSort<int>::sort(vout10);
        }
        PROFILE_STOP(bubble_1);

        cout << "Bubble sort #2..." << endl;
        PROFILE_START(bubble_2);
        auto vout11 = vin;
        for (int i = 0; i < T; i++) {
            vout11 = vin;
            BubbleSort<int>::sort(vout11);
        }
        PROFILE_STOP(bubble_2);
        */

        //cout << "Insertion sort..." << endl;
        //PROFILE_START(insertion);
        //auto voutIns = vin;
        //for (int i = 0; i < T; i++) {
        //    voutIns = vin;
        //    InsertionSort<int>::sort(voutIns);
        //}
        //PROFILE_STOP(insertion);

        cout << "Shellsort..." << endl;
        PROFILE_START(shell);
        auto voutShell = vin;
        for (int i = 0; i < T; i++) {
            voutShell = vin;
            ShellSort<int>::sort(voutShell);
        }
        PROFILE_STOP(shell);

        cout << "Heap sort..." << endl;
        PROFILE_START(heap);
        auto vout2 = vin;
        for (int i = 0; i < T; i++) {
            vout2 = vin;
            MaxHeap<int>::sort(vout2);
        }
        PROFILE_STOP(heap);

        cout << "Merge sort..." << endl;
        PROFILE_START(merge);
        auto vout3 = vin;
        for (int i = 0; i < T; i++) {
            vout3 = vin;
            MergeSort<int>::sort(vout3);
        }
        PROFILE_STOP(merge);

        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE_START(merge_bottom_up);
        auto vout3_1 = vin;
        for (int i = 0; i < T; i++) {
            vout3_1 = vin;
            MergeSortBottomUp<int>::sort(vout3_1);
        }
        PROFILE_STOP(merge_bottom_up);

        cout << "Merge sort (enhanced)..." << endl;
        PROFILE_START(merge_enhanced);
        auto vout3_2 = vin;
        for (int i = 0; i < T; i++) {
            vout3_2 = vin;
            MergeSortEnhanced<int>::sort(vout3_2);
        }
        PROFILE_STOP(merge_enhanced);

        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE_START(merge_enhanced2);
        auto vout3_3 = vin;
        for (int i = 0; i < T; i++) {
            vout3_3 = vin;
            MergeSortEnhanced2<int>::sort(vout3_3);
        }
        PROFILE_STOP(merge_enhanced2);

        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE_START(merge_enhanced3);
        auto vout3_4 = vin;
        for (int i = 0; i < T; i++) {
            vout3_4 = vin;
            MergeSortEnhanced3<int>::sort(vout3_4);
        }
        PROFILE_STOP(merge_enhanced3);

        cout << "Quick sort..." << endl;
        PROFILE_START(quick);
        auto vout4 = vin;
        for (int i = 0; i < T; i++) {
            vout4 = vin;
            QuickSort<int>::sort(vout4);
        }
        PROFILE_STOP(quick);

        cout << "Quick sort with MED3..." << endl;
        PROFILE_START(quick_med3);
        auto vout4_1 = vin;
        for (int i = 0; i < T; i++) {
            vout4_1 = vin;
            QuickSort<int>::sort3(vout4_1);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto vout4_2 = vin;
        for (int i = 0; i < T; i++) {
            vout4_2 = vin;
            QuickSort3way<int>::sort(vout4_2);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto vout4_3 = vin;
        for (int i = 0; i < T; i++) {
            vout4_3 = vin;
            QuickSort3way<int>::sort2(vout4_3);
        }
        PROFILE_STOP(quick_3way_med3);

        if (voutShell != vout2
            || voutShell != vout3
            || voutShell != vout3_1
            || voutShell != vout3_2
            || voutShell != vout3_3
            || voutShell != vout3_4
            || voutShell != vout4
            || voutShell != vout4_1
            || voutShell != vout4_2
            || voutShell != vout4_3)
            cout << "ERROR : mismatched!" << endl;
    }
    cout << "--- speed test #2" << endl;
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

        /*
        cout << "Bubble sort #1..." << endl;
        PROFILE_START(bubble_1);
        auto vout10 = vin;
        for (int i = 0; i < T; i++) {
            vout10 = vin;
            BubbleSort<int>::sort(vout10);
        }
        PROFILE_STOP(bubble_1);

        cout << "Bubble sort #2..." << endl;
        PROFILE_START(bubble_2);
        auto vout11 = vin;
        for (int i = 0; i < T; i++) {
            vout11 = vin;
            BubbleSort<int>::sort(vout11);
        }
        PROFILE_STOP(bubble_2);
        */

        //cout << "Insertion sort..." << endl;
        //PROFILE_START(insertion);
        //auto voutIns = vin;
        //for (int i = 0; i < T; i++) {
        //    voutIns = vin;
        //    InsertionSort<int>::sort(voutIns);
        //}
        //PROFILE_STOP(insertion);

        cout << "Shellsort..." << endl;
        PROFILE_START(shell);
        auto voutShell = vin;
        for (int i = 0; i < T; i++) {
            voutShell = vin;
            ShellSort<int>::sort(voutShell);
        }
        PROFILE_STOP(shell);

        cout << "Heap sort..." << endl;
        PROFILE_START(heap);
        auto vout1 = vin;
        for (int i = 0; i < T; i++) {
            vout1 = vin;
            MaxHeap<int>::sort(vout1);
        }
        PROFILE_STOP(heap);

        cout << "Merge sort..." << endl;
        PROFILE_START(merge);
        auto vout2 = vin;
        for (int i = 0; i < T; i++) {
            vout2 = vin;
            MergeSort<int>::sort(vout2);
        }
        PROFILE_STOP(merge);

        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE_START(merge_bottom_up);
        auto vout2_1 = vin;
        for (int i = 0; i < T; i++) {
            vout2_1 = vin;
            MergeSortBottomUp<int>::sort(vout2_1);
        }
        PROFILE_STOP(merge_bottom_up);

        cout << "Merge sort (enhanced)..." << endl;
        PROFILE_START(merge_enhanced);
        auto vout2_2 = vin;
        for (int i = 0; i < T; i++) {
            vout2_2 = vin;
            MergeSortEnhanced<int>::sort(vout2_2);
        }
        PROFILE_STOP(merge_enhanced);

        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE_START(merge_enhanced2);
        auto vout2_3 = vin;
        for (int i = 0; i < T; i++) {
            vout2_3 = vin;
            MergeSortEnhanced2<int>::sort(vout2_3);
        }
        PROFILE_STOP(merge_enhanced2);

        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE_START(merge_enhanced3);
        auto vout2_4 = vin;
        for (int i = 0; i < T; i++) {
            vout2_4 = vin;
            MergeSortEnhanced3<int>::sort(vout2_4);
        }
        PROFILE_STOP(merge_enhanced3);

        cout << "Quick sort..." << endl;
        PROFILE_START(quick);
        auto vout3 = vin;
        for (int i = 0; i < T; i++) {
            vout3 = vin;
            QuickSort<int>::sort(vout3);
        }
        PROFILE_STOP(quick);

        cout << "Quick sort with MED3..." << endl;
        PROFILE_START(quick_med3);
        auto vout3_1 = vin;
        for (int i = 0; i < T; i++) {
            vout3_1 = vin;
            QuickSort<int>::sort3(vout3_1);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto vout3_2 = vin;
        for (int i = 0; i < T; i++) {
            vout3_2 = vin;
            QuickSort3way<int>::sort(vout3_2);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto vout3_3 = vin;
        for (int i = 0; i < T; i++) {
            vout3_3 = vin;
            QuickSort3way<int>::sort2(vout3_3);
        }
        PROFILE_STOP(quick_3way_med3);

        if (voutShell != vout1
            || voutShell != vout2
            || voutShell != vout2_1
            || voutShell != vout2_2
            || voutShell != vout2_3
            || voutShell != vout2_4
            || voutShell != vout3
            || voutShell != vout3_1
            || voutShell != vout3_2
            || voutShell != vout3_3)
            cout << "ERROR : mismatched!" << endl;
    }
    cout << "--- speed test #3" << endl;
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

        /*
        cout << "Bubble sort #1..." << endl;
        PROFILE_START(bubble_1);
        auto vout10 = vin;
        for (int i = 0; i < T; i++) {
            vout10 = vin;
            BubbleSort<int>::sort(vout10);
        }
        PROFILE_STOP(bubble_1);

        cout << "Bubble sort #2..." << endl;
        PROFILE_START(bubble_2);
        auto vout11 = vin;
        for (int i = 0; i < T; i++) {
            vout11 = vin;
            BubbleSort<int>::sort(vout11);
        }
        PROFILE_STOP(bubble_2);
        */

        //cout << "Insertion sort..." << endl;
        //PROFILE_START(insertion);
        //auto voutIns = vin;
        //for (int i = 0; i < T; i++) {
        //    voutIns = vin;
        //    InsertionSort<int>::sort(voutIns);
        //}
        //PROFILE_STOP(insertion);

        cout << "Shellsort..." << endl;
        PROFILE_START(shell);
        auto voutShell = vin;
        for (int i = 0; i < T; i++) {
            voutShell = vin;
            ShellSort<int>::sort(voutShell);
        }
        PROFILE_STOP(shell);

        cout << "Heap sort..." << endl;
        PROFILE_START(heap);
        auto vout1 = vin;
        for (int i = 0; i < T; i++) {
            vout1 = vin;
            MaxHeap<int>::sort(vout1);
        }
        PROFILE_STOP(heap);

        cout << "Merge sort..." << endl;
        PROFILE_START(merge);
        auto vout2 = vin;
        for (int i = 0; i < T; i++) {
            vout2 = vin;
            MergeSort<int>::sort(vout2);
        }
        PROFILE_STOP(merge);

        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE_START(merge_bottom_up);
        auto vout2_1 = vin;
        for (int i = 0; i < T; i++) {
            vout2_1 = vin;
            MergeSortBottomUp<int>::sort(vout2_1);
        }
        PROFILE_STOP(merge_bottom_up);

        cout << "Merge sort (enhanced)..." << endl;
        PROFILE_START(merge_enhanced);
        auto vout2_2 = vin;
        for (int i = 0; i < T; i++) {
            vout2_2 = vin;
            MergeSortEnhanced<int>::sort(vout2_2);
        }
        PROFILE_STOP(merge_enhanced);

        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE_START(merge_enhanced2);
        auto vout2_3 = vin;
        for (int i = 0; i < T; i++) {
            vout2_3 = vin;
            MergeSortEnhanced2<int>::sort(vout2_3);
        }
        PROFILE_STOP(merge_enhanced2);

        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE_START(merge_enhanced3);
        auto vout2_4 = vin;
        for (int i = 0; i < T; i++) {
            vout2_4 = vin;
            MergeSortEnhanced3<int>::sort(vout2_4);
        }
        PROFILE_STOP(merge_enhanced3);

        cout << "Quick sort..." << endl;
        PROFILE_START(quick);
        auto vout3 = vin;
        for (int i = 0; i < T; i++) {
            vout3 = vin;
            QuickSort<int>::sort(vout3);
        }
        PROFILE_STOP(quick);

        cout << "Quick sort with MED3..." << endl;
        PROFILE_START(quick_med3);
        auto vout3_1 = vin;
        for (int i = 0; i < T; i++) {
            vout3_1 = vin;
            QuickSort<int>::sort3(vout3_1);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto vout3_2 = vin;
        for (int i = 0; i < T; i++) {
            vout3_2 = vin;
            QuickSort3way<int>::sort(vout3_2);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto vout3_3 = vin;
        for (int i = 0; i < T; i++) {
            vout3_3 = vin;
            QuickSort3way<int>::sort2(vout3_3);
        }
        PROFILE_STOP(quick_3way_med3);

        if (voutShell != vout1
            || voutShell != vout2
            || voutShell != vout2_1
            || voutShell != vout2_2
            || voutShell != vout2_3
            || voutShell != vout2_4
            || voutShell != vout3
            || voutShell != vout3_1
            || voutShell != vout3_2
            || voutShell != vout3_3)
            cout << "ERROR : mismatched!" << endl;
    }
    cout << "OK!" << endl;

//#ifndef _DEBUG
#if 0
    cout << "--- speed test per N " << endl;
    {
        //for (int N = 10; N <= 10'000'000; N *= 10) {
        int N = 10'000'000;
        {
            vector<int> vin(N);
            for (int i = 0; i < N; i++)
                vin[i] = RandInt32::get();

            cout << "-- N = " << N << endl;

            //auto voutIns = vin;
            //PROFILE_HI_START(insertion);
            //InsertionSort<int>::sort(voutIns);
            //PROFILE_HI_STOP(insertion);

            auto voutShell = vin;
            PROFILE_HI_START(shell);
            ShellSort<int>::sort(voutShell);
            PROFILE_HI_STOP(shell);

            auto voutHeap = vin;
            PROFILE_HI_START(heap);
            MaxHeap<int>::sort(voutHeap);
            PROFILE_HI_STOP(heap);

            auto voutMerge = vin;
            PROFILE_HI_START(merge);
            MergeSort<int>::sort(voutMerge);
            PROFILE_HI_STOP(merge);

            auto voutMerge2 = vin;
            PROFILE_HI_START(merge_bottom_up);
            MergeSortBottomUp<int>::sort(voutMerge2);
            PROFILE_HI_STOP(merge_bottom_up);

            auto voutMerge3 = vin;
            PROFILE_HI_START(merge_enhanced);
            MergeSortEnhanced<int>::sort(voutMerge3);
            PROFILE_HI_STOP(merge_enhanced);

            auto voutMerge4 = vin;
            PROFILE_HI_START(merge_enhanced2);
            MergeSortEnhanced2<int>::sort(voutMerge4);
            PROFILE_HI_STOP(merge_enhanced2);

            auto voutMerge5 = vin;
            PROFILE_HI_START(merge_enhanced3);
            MergeSortEnhanced3<int>::sort(voutMerge5);
            PROFILE_HI_STOP(merge_enhanced3);

            auto voutQuick = vin;
            PROFILE_HI_START(quick);
            QuickSort<int>::sort(voutQuick);
            PROFILE_HI_STOP(quick);

            if (//voutIns != voutShell ||
                voutShell != voutHeap ||
                voutHeap != voutMerge ||
                voutMerge != voutQuick ||
                voutMerge2 != voutQuick ||
                voutMerge3 != voutQuick)
                cout << "ERROR : mismatched!" << endl;
        }
    }
#endif
}
