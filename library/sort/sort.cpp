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
#include "sortTim.h"
#include "sortQuick.h"
#include "sortQuick3way.h"
#include "sortIntro.h"

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

            auto voutHeap = vin;
            MaxHeap<int>::sort(voutHeap);

            auto voutMerge1 = vin;
            MergeSort<int>::sort(voutMerge1);
            auto voutMerge2 = vin;
            MergeSortBottomUp<int>::sort(voutMerge2);
            auto voutMerge3 = vin;
            MergeSortEnhanced<int>::sort(voutMerge3);
            auto voutMerge4 = vin;
            MergeSortEnhanced2<int>::sort(voutMerge4);
            auto voutMerge5 = vin;
            MergeSortEnhanced3<int>::sort(voutMerge5);

            auto voutQuick1 = vin;
            QuickSort<int>::sort(voutQuick1);
            auto voutQuick2 = vin;
            QuickSort<int>::sort2(voutQuick2);
            auto voutQuick3 = vin;
            QuickSort<int>::sort3(voutQuick3);

            auto voutIntro1 = vin;
            IntroSort<int>::sort(voutIntro1);
            auto voutIntro2 = vin;
            IntroSort<int>::sort2(voutIntro2);
            auto voutIntro3 = vin;
            IntroSort<int>::sort3(voutIntro3);

            auto voutQuick3way1 = vin;
            QuickSort3way<int>::sort(voutQuick3way1);
            auto voutQuick3way2 = vin;
            QuickSort3way<int>::sort2(voutQuick3way2);

            auto voutTim = vin;
            TimSort<int>::sort(voutTim);

            if (vout00 != voutHeap || vout01 != voutHeap
                || voutSel != voutHeap
                || voutIns != voutHeap
                || voutShell != voutHeap
                || voutMerge1 != voutHeap || voutMerge2 != voutHeap || voutMerge3 != voutHeap || voutMerge4 != voutHeap || voutMerge5 != voutHeap
                || voutQuick1 != voutHeap || voutQuick2 != voutHeap || voutQuick3 != voutHeap
                || voutIntro1 != voutHeap || voutIntro2 != voutHeap || voutIntro3 != voutHeap
                || voutQuick3way1 != voutHeap || voutQuick3way2 != voutHeap
                || voutTim != voutHeap)
                cout << "ERROR : mismatched!" << endl;

            assert(vout00 == voutHeap);
            assert(vout01 == voutHeap);
            assert(voutSel == voutHeap);
            assert(voutIns == voutHeap);
            assert(voutShell == voutHeap);
            assert(voutMerge1 == voutHeap);
            assert(voutMerge2 == voutHeap);
            assert(voutMerge3 == voutHeap);
            assert(voutMerge4 == voutHeap);
            assert(voutMerge5 == voutHeap);
            assert(voutQuick1 == voutHeap);
            assert(voutQuick2 == voutHeap);
            assert(voutQuick3 == voutHeap);
            assert(voutQuick3way1 == voutHeap);
            assert(voutQuick3way2 == voutHeap);
            assert(voutIntro1 == voutHeap);
            assert(voutIntro2 == voutHeap);
            assert(voutIntro3 == voutHeap);
            assert(vout5 == voutHeap);
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
        auto voutBubble1 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble1 = vin;
            BubbleSort<int>::sort(voutBubble1);
        }
        PROFILE_STOP(bubble_1);

        cout << "Bubble sort #2..." << endl;
        PROFILE_START(bubble_2);
        auto voutBubble2 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble2 = vin;
            BubbleSort<int>::sort(voutBubble2);
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
        auto voutHeap = vin;
        for (int i = 0; i < T; i++) {
            voutHeap = vin;
            MaxHeap<int>::sort(voutHeap);
        }
        PROFILE_STOP(heap);

        cout << "Merge sort..." << endl;
        PROFILE_START(merge);
        auto voutMerge1 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge1 = vin;
            MergeSort<int>::sort(voutMerge1);
        }
        PROFILE_STOP(merge);

        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE_START(merge_bottom_up);
        auto voutMerge2 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge2 = vin;
            MergeSortBottomUp<int>::sort(voutMerge2);
        }
        PROFILE_STOP(merge_bottom_up);

        cout << "Merge sort (enhanced)..." << endl;
        PROFILE_START(merge_enhanced);
        auto voutMerge3 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge3 = vin;
            MergeSortEnhanced<int>::sort(voutMerge3);
        }
        PROFILE_STOP(merge_enhanced);

        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE_START(merge_enhanced2);
        auto voutMerge4 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge4 = vin;
            MergeSortEnhanced2<int>::sort(voutMerge4);
        }
        PROFILE_STOP(merge_enhanced2);

        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE_START(merge_enhanced3);
        auto voutMerge5 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge5 = vin;
            MergeSortEnhanced3<int>::sort(voutMerge5);
        }
        PROFILE_STOP(merge_enhanced3);

        cout << "Quick sort..." << endl;
        PROFILE_START(quick1);
        auto voutQuick1 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick1 = vin;
            QuickSort<int>::sort(voutQuick1);
        }
        PROFILE_STOP(quick1);

        cout << "Quick sort 2..." << endl;
        PROFILE_START(quick2);
        auto voutQuick2 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick2 = vin;
            QuickSort<int>::sort(voutQuick2);
        }
        PROFILE_STOP(quick2);

        cout << "Quick sort with MED3..." << endl;
        PROFILE_START(quick_med3);
        auto voutQuick3 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3 = vin;
            QuickSort<int>::sort3(voutQuick3);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto voutQuick3way1 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3way1 = vin;
            QuickSort3way<int>::sort(voutQuick3way1);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto voutQuick3way2 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3way2 = vin;
            QuickSort3way<int>::sort2(voutQuick3way2);
        }
        PROFILE_STOP(quick_3way_med3);

        cout << "Introsort 1..." << endl;
        PROFILE_START(intro1);
        auto voutIntro1 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro1 = vin;
            IntroSort<int>::sort(voutIntro1);
        }
        PROFILE_STOP(intro1);

        cout << "Introsort 2..." << endl;
        PROFILE_START(intro2);
        auto voutIntro2 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro2 = vin;
            IntroSort<int>::sort(voutIntro2);
        }
        PROFILE_STOP(intro2);

        cout << "Introsort 3..." << endl;
        PROFILE_START(intro3);
        auto voutIntro3 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro3 = vin;
            IntroSort<int>::sort(voutIntro3);
        }
        PROFILE_STOP(intro3);

        cout << "Tim sort..." << endl;
        PROFILE_START(tim);
        auto voutTim = vin;
        for (int i = 0; i < T; i++) {
            voutTim = vin;
            TimSort<int>::sort(voutTim);
        }
        PROFILE_STOP(tim);

        if (voutShell != voutHeap
            || voutShell != voutMerge1
            || voutShell != voutMerge2
            || voutShell != voutMerge3
            || voutShell != voutMerge4
            || voutShell != voutMerge5
            || voutShell != voutQuick1
            || voutShell != voutQuick2
            || voutShell != voutQuick3
            || voutShell != voutQuick3way1
            || voutShell != voutQuick3way2
            || voutShell != voutIntro1
            || voutShell != voutIntro2
            || voutShell != voutIntro3
            || voutShell != voutTim)
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
        auto voutBubble1 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble1 = vin;
            BubbleSort<int>::sort(voutBubble1);
        }
        PROFILE_STOP(bubble_1);

        cout << "Bubble sort #2..." << endl;
        PROFILE_START(bubble_2);
        auto voutBubble2 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble2 = vin;
            BubbleSort<int>::sort(voutBubble2);
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
        PROFILE_START(quick1);
        auto voutQuick1 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick1 = vin;
            QuickSort<int>::sort(voutQuick1);
        }
        PROFILE_STOP(quick1);

        cout << "Quick sort 2..." << endl;
        PROFILE_START(quick2);
        auto voutQuick2 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick2 = vin;
            QuickSort<int>::sort(voutQuick2);
        }
        PROFILE_STOP(quick2);

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

        cout << "Introsort 1..." << endl;
        PROFILE_START(intro1);
        auto voutIntro1 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro1 = vin;
            IntroSort<int>::sort(voutIntro1);
        }
        PROFILE_STOP(intro1);

        cout << "Introsort 2..." << endl;
        PROFILE_START(intro2);
        auto voutIntro2 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro2 = vin;
            IntroSort<int>::sort(voutIntro2);
        }
        PROFILE_STOP(intro2);

        cout << "Introsort 3..." << endl;
        PROFILE_START(intro3);
        auto voutIntro3 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro3 = vin;
            IntroSort<int>::sort(voutIntro3);
        }
        PROFILE_STOP(intro3);

        cout << "Tim sort..." << endl;
        PROFILE_START(tim);
        auto voutTim = vin;
        for (int i = 0; i < T; i++) {
            voutTim = vin;
            TimSort<int>::sort(voutTim);
        }
        PROFILE_STOP(tim);

        if (voutShell != vout1
            || voutShell != vout2
            || voutShell != vout2_1
            || voutShell != vout2_2
            || voutShell != vout2_3
            || voutShell != vout2_4
            || voutShell != voutQuick1
            || voutShell != voutQuick2
            || voutShell != voutIntro1
            || voutShell != voutIntro2
            || voutShell != voutIntro3
            || voutShell != vout3_1
            || voutShell != vout3_2
            || voutShell != vout3_3
            || voutShell != voutTim)
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
        auto voutBubble1 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble1 = vin;
            BubbleSort<int>::sort(voutBubble1);
        }
        PROFILE_STOP(bubble_1);

        cout << "Bubble sort #2..." << endl;
        PROFILE_START(bubble_2);
        auto voutBubble2 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble2 = vin;
            BubbleSort<int>::sort(voutBubble2);
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
        auto voutHeap = vin;
        for (int i = 0; i < T; i++) {
            voutHeap = vin;
            MaxHeap<int>::sort(voutHeap);
        }
        PROFILE_STOP(heap);

        cout << "Merge sort..." << endl;
        PROFILE_START(merge);
        auto voutMerge1 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge1 = vin;
            MergeSort<int>::sort(voutMerge1);
        }
        PROFILE_STOP(merge);

        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE_START(merge_bottom_up);
        auto voutMerge2 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge2 = vin;
            MergeSortBottomUp<int>::sort(voutMerge2);
        }
        PROFILE_STOP(merge_bottom_up);

        cout << "Merge sort (enhanced)..." << endl;
        PROFILE_START(merge_enhanced);
        auto voutMerge3 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge3 = vin;
            MergeSortEnhanced<int>::sort(voutMerge3);
        }
        PROFILE_STOP(merge_enhanced);

        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE_START(merge_enhanced2);
        auto voutMerge4 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge4 = vin;
            MergeSortEnhanced2<int>::sort(voutMerge4);
        }
        PROFILE_STOP(merge_enhanced2);

        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE_START(merge_enhanced3);
        auto voutMerge5 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge5 = vin;
            MergeSortEnhanced3<int>::sort(voutMerge5);
        }
        PROFILE_STOP(merge_enhanced3);

        cout << "Quick sort..." << endl;
        PROFILE_START(quick1);
        auto voutQuick1 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick1 = vin;
            QuickSort<int>::sort(voutQuick1);
        }
        PROFILE_STOP(quick1);

        cout << "Quick sort 2..." << endl;
        PROFILE_START(quick2);
        auto voutQuick2 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick2 = vin;
            QuickSort<int>::sort(voutQuick2);
        }
        PROFILE_STOP(quick2);

        cout << "Quick sort with MED3..." << endl;
        PROFILE_START(quick_med3);
        auto voutQuick3 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3 = vin;
            QuickSort<int>::sort3(voutQuick3);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto voutQuick3way1 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3way1 = vin;
            QuickSort3way<int>::sort(voutQuick3way1);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto voutQuick3way2 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3way2 = vin;
            QuickSort3way<int>::sort2(voutQuick3way2);
        }
        PROFILE_STOP(quick_3way_med3);

        cout << "Introsort 1..." << endl;
        PROFILE_START(intro1);
        auto voutIntro1 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro1 = vin;
            IntroSort<int>::sort(voutIntro1);
        }
        PROFILE_STOP(intro1);

        cout << "Introsort 2..." << endl;
        PROFILE_START(intro2);
        auto voutIntro2 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro2 = vin;
            IntroSort<int>::sort(voutIntro2);
        }
        PROFILE_STOP(intro2);

        cout << "Introsort 3..." << endl;
        PROFILE_START(intro3);
        auto voutIntro3 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro3 = vin;
            IntroSort<int>::sort(voutIntro3);
        }
        PROFILE_STOP(intro3);

        cout << "Tim sort..." << endl;
        PROFILE_START(tim);
        auto voutTim = vin;
        for (int i = 0; i < T; i++) {
            voutTim = vin;
            TimSort<int>::sort(voutTim);
        }
        PROFILE_STOP(tim);

        if (voutShell != voutHeap
            || voutShell != voutMerge1
            || voutShell != voutMerge2
            || voutShell != voutMerge3
            || voutShell != voutMerge4
            || voutShell != voutMerge5
            || voutShell != voutQuick1
            || voutShell != voutQuick2
            || voutShell != voutQuick3
            || voutShell != voutQuick3way1
            || voutShell != voutQuick3way2
            || voutShell != voutIntro1
            || voutShell != voutIntro2
            || voutShell != voutIntro3
            || voutShell != voutTim)
            cout << "ERROR : mismatched!" << endl;
    }
    cout << "OK!" << endl;

    cout << "--- speed test #4" << endl;
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

        /*
        cout << "Bubble sort #1..." << endl;
        PROFILE_START(bubble_1);
        auto voutBubble1 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble1 = vin;
            BubbleSort<int>::sort(voutBubble1);
        }
        PROFILE_STOP(bubble_1);

        cout << "Bubble sort #2..." << endl;
        PROFILE_START(bubble_2);
        auto voutBubble2 = vin;
        for (int i = 0; i < T; i++) {
            voutBubble2 = vin;
            BubbleSort<int>::sort(voutBubble2);
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
        auto voutHeap = vin;
        for (int i = 0; i < T; i++) {
            voutHeap = vin;
            MaxHeap<int>::sort(voutHeap);
        }
        PROFILE_STOP(heap);

        cout << "Merge sort..." << endl;
        PROFILE_START(merge);
        auto voutMerge1 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge1 = vin;
            MergeSort<int>::sort(voutMerge1);
        }
        PROFILE_STOP(merge);

        cout << "Merge sort (bottom-up)..." << endl;
        PROFILE_START(merge_bottom_up);
        auto voutMerge2 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge2 = vin;
            MergeSortBottomUp<int>::sort(voutMerge2);
        }
        PROFILE_STOP(merge_bottom_up);

        cout << "Merge sort (enhanced)..." << endl;
        PROFILE_START(merge_enhanced);
        auto voutMerge3 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge3 = vin;
            MergeSortEnhanced<int>::sort(voutMerge3);
        }
        PROFILE_STOP(merge_enhanced);

        cout << "Merge sort (enhanced2)..." << endl;
        PROFILE_START(merge_enhanced2);
        auto voutMerge4 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge4 = vin;
            MergeSortEnhanced2<int>::sort(voutMerge4);
        }
        PROFILE_STOP(merge_enhanced2);

        cout << "Merge sort (enhanced3)..." << endl;
        PROFILE_START(merge_enhanced3);
        auto voutMerge5 = vin;
        for (int i = 0; i < T; i++) {
            voutMerge5 = vin;
            MergeSortEnhanced3<int>::sort(voutMerge5);
        }
        PROFILE_STOP(merge_enhanced3);

        cout << "Quick sort..." << endl;
        PROFILE_START(quick1);
        auto voutQuick1 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick1 = vin;
            QuickSort<int>::sort(voutQuick1);
        }
        PROFILE_STOP(quick1);

        cout << "Quick sort 2..." << endl;
        PROFILE_START(quick2);
        auto voutQuick2 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick2 = vin;
            QuickSort<int>::sort(voutQuick2);
        }
        PROFILE_STOP(quick2);

        cout << "Quick sort with MED3..." << endl;
        PROFILE_START(quick_med3);
        auto voutQuick3 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3 = vin;
            QuickSort<int>::sort3(voutQuick3);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto voutQuick3way1 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3way1 = vin;
            QuickSort3way<int>::sort(voutQuick3way1);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto voutQuick3way2 = vin;
        for (int i = 0; i < T; i++) {
            voutQuick3way2 = vin;
            QuickSort3way<int>::sort2(voutQuick3way2);
        }
        PROFILE_STOP(quick_3way_med3);

        cout << "Introsort 1..." << endl;
        PROFILE_START(intro1);
        auto voutIntro1 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro1 = vin;
            IntroSort<int>::sort(voutIntro1);
        }
        PROFILE_STOP(intro1);

        cout << "Introsort 2..." << endl;
        PROFILE_START(intro2);
        auto voutIntro2 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro2 = vin;
            IntroSort<int>::sort(voutIntro2);
        }
        PROFILE_STOP(intro2);

        cout << "Introsort 3..." << endl;
        PROFILE_START(intro3);
        auto voutIntro3 = vin;
        for (int i = 0; i < T; i++) {
            voutIntro3 = vin;
            IntroSort<int>::sort(voutIntro3);
        }
        PROFILE_STOP(intro3);

        cout << "Tim sort..." << endl;
        PROFILE_START(tim);
        auto voutTim = vin;
        for (int i = 0; i < T; i++) {
            voutTim = vin;
            TimSort<int>::sort(voutTim);
        }
        PROFILE_STOP(tim);

        if (voutShell != voutHeap
            || voutShell != voutMerge1
            || voutShell != voutMerge2
            || voutShell != voutMerge3
            || voutShell != voutMerge4
            || voutShell != voutMerge5
            || voutShell != voutQuick1
            || voutShell != voutQuick2
            || voutShell != voutQuick3
            || voutShell != voutQuick3way1
            || voutShell != voutQuick3way2
            || voutShell != voutIntro1
            || voutShell != voutIntro2
            || voutShell != voutIntro3
            || voutShell != voutTim)
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

            auto voutTim = vin;
            PROFILE_START(tim);
            TimSort<int>::sort(voutTim);
            PROFILE_STOP(tim);

            if (//voutIns != voutShell ||
                voutShell != voutHeap ||
                voutHeap != voutMerge ||
                voutMerge != voutQuick ||
                voutMerge2 != voutQuick ||
                voutMerge3 != voutQuick ||
                voutShell != voutTim)
                cout << "ERROR : mismatched!" << endl;
        }
    }
#endif
}
