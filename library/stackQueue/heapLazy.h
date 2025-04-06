#pragma once

// heap with lazy pop operation
// - default is max heap
template<typename T = int, typename CompareT = less<T>>
struct HeapLazy {
    int size() const {
        return int(addQ.size()) - int(delQ.size());
    }

    bool empty() const {
        return size() <= 0;
    }

    long long totalSum() const {
        return sum;
    }


    // O(logN)
    void push(T val) {
        addQ.push(val);
        sum += val;
    }

    // precondition: val must exist
    // O(logN)
    void pop(T val) {
        delQ.push(val);
        sum -= val;
    }

    // worst: O(NlogN), amortized: O(logN)
    T pop() {
        auto x = top();
        pop(x);
        return x;
    }

    // best: O(1), worst: O(NlogN)
    // amortized: O(logN)
    T top() {
        while (!addQ.empty() && !delQ.empty() && addQ.top() == delQ.top()) {
            addQ.pop();
            delQ.pop();
        }
        return addQ.top();
    }

private:
    long long sum = 0;
    priority_queue<T, vector<T>, CompareT> addQ;
    priority_queue<T, vector<T>, CompareT> delQ;
};
