#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

#include "sortBubble.h"
#include "sortSelection.h"
#include "sortInsertion.h"
#include "sortHeap.h"
#include "sortMerge.h"
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

            auto vout1 = vin;
            MaxHeap<int>::sort(vout1);

            auto vout2 = vin;
            MergeSort<int>::sort(vout2);

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
                || vout2 != vout1
                || vout3 != vout1 || vout3_3 != vout1
                || vout4 != vout1 || vout4_2 != vout1)
                cout << "ERROR : mismatched!" << endl;

            assert(vout00 == vout1);
            assert(vout01 == vout1);
            assert(voutSel == vout1);
            assert(voutIns == vout1);
            assert(vout2 == vout1);
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

        cout << "Insertion sort..." << endl;
        PROFILE_START(insertion);
        auto voutIns = vin;
        for (int i = 0; i < T; i++) {
            voutIns = vin;
            InsertionSort<int>::sort(voutIns);
        }
        PROFILE_STOP(insertion);

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
        auto vout4_3 = vin;
        for (int i = 0; i < T; i++) {
            vout4_3 = vin;
            QuickSort<int>::sort3(vout4_3);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto vout5 = vin;
        for (int i = 0; i < T; i++) {
            vout5 = vin;
            QuickSort3way<int>::sort(vout5);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto vout5_2 = vin;
        for (int i = 0; i < T; i++) {
            vout5_2 = vin;
            QuickSort3way<int>::sort2(vout5_2);
        }
        PROFILE_STOP(quick_3way_med3);
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

        cout << "Insertion sort..." << endl;
        PROFILE_START(insertion);
        auto voutIns = vin;
        for (int i = 0; i < T; i++) {
            voutIns = vin;
            InsertionSort<int>::sort(voutIns);
        }
        PROFILE_STOP(insertion);

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
        auto vout3_3 = vin;
        for (int i = 0; i < T; i++) {
            vout3_3 = vin;
            QuickSort<int>::sort3(vout3_3);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto vout4 = vin;
        for (int i = 0; i < T; i++) {
            vout4 = vin;
            QuickSort3way<int>::sort(vout4);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto vout4_2 = vin;
        for (int i = 0; i < T; i++) {
            vout4_2 = vin;
            QuickSort3way<int>::sort2(vout4_2);
        }
        PROFILE_STOP(quick_3way_med3);
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

        cout << "Insertion sort..." << endl;
        PROFILE_START(insertion);
        auto voutIns = vin;
        for (int i = 0; i < T; i++) {
            voutIns = vin;
            InsertionSort<int>::sort(voutIns);
        }
        PROFILE_STOP(insertion);

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
        auto vout3_3 = vin;
        for (int i = 0; i < T; i++) {
            vout3_3 = vin;
            QuickSort<int>::sort3(vout3_3);
        }
        PROFILE_STOP(quick_med3);

        cout << "3-way Quick sort..." << endl;
        PROFILE_START(quick_3way);
        auto vout4 = vin;
        for (int i = 0; i < T; i++) {
            vout4 = vin;
            QuickSort3way<int>::sort(vout4);
        }
        PROFILE_STOP(quick_3way);

        cout << "3-way Quick sort with MED3..." << endl;
        PROFILE_START(quick_3way_med3);
        auto vout4_2 = vin;
        for (int i = 0; i < T; i++) {
            vout4_2 = vin;
            QuickSort3way<int>::sort2(vout4_2);
        }
        PROFILE_STOP(quick_3way_med3);
    }
    cout << "OK!" << endl;
}
