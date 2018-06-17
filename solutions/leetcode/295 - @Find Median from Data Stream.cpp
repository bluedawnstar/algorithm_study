class MedianFinder {
    priority_queue<int> small, large;

public:
    /** initialize your data structure here. */
    MedianFinder() {

    }

    // O(logn)
    void addNum(int num) {
        small.push(num);
        large.push(-small.top());
        small.pop();
        if (small.size() < large.size()) {
            small.push(-large.top());
            large.pop();
        }
    }

    // O(1)
    double findMedian() {
        return small.size() > large.size() ? small.top()
            : (small.top() - large.top()) / 2.0;
    }
};

class MedianFinder {
    vector<int> v;

public:
    /** initialize your data structure here. */
    MedianFinder() {

    }

    // O(n)
    void addNum(int num) {
        v.insert(lower_bound(v.begin(), v.end(), num), num);
    }

    // O(1)
    double findMedian() {
        int half = (int)v.size() / 2;
        if (v.size() & 1)
            return v[half];
        else
            return (v[half] + v[half - 1]) / 2.0;
    }
};

/**
* Your MedianFinder object will be instantiated and called as such:
* MedianFinder obj = new MedianFinder();
* obj.addNum(num);
* double param_2 = obj.findMedian();
*/