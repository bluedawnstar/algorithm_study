#pragma once

// https://e-maxx-eng.appspot.com/data_structures/stack_queue_modification.html

template <typename T>
struct MinQueue {
    deque<T> queue;
    deque<T> queueMin;

    size_t size() const {
        return queue.size();
    }

    bool empty() const {
        return queue.empty();
    }

    T front() const {
        return queue.front();
    }

    T min() const {
        return queueMin.front();
    }

    void push(T x) {
        while (!queueMin.empty() && queueMin.back() > x)
            queueMin.pop_back();
        queueMin.push_back(x);

        queue.push_back(x);
    }

    void pop() {
        if (!queueMin.empty() && queueMin.front() == queue.front())
            queueMin.pop_front();

        queue.pop_front();
    }
};

template <typename T>
struct MinQueue2 {
    deque<pair<T, T>> queue;
    int added = 0;
    int removed = 0;

    size_t size() const {
        return size_t(added - removed);
    }

    bool empty() const {
        return added <= removed;
    }

    T min() const {
        return queue.front().first;
    }

    void push(T x) {
        while (!queue.empty() && queue.back().first > x)
            queue.pop_back();
        queue.emplace_back(x, added++);
    }

    void pop() {
        if (!queue.empty() && queue.front().second == removed++)
            queue.pop_front();
    }
};

template <typename T>
struct MinQueue3 {
    stack<pair<T, T>> stk1;
    stack<pair<T, T>> stk2;

    size_t size() const {
        return stk1.size() + stk2.size();
    }

    bool empty() const {
        return stk1.empty() && stk2.empty();
    }

    T min() const {
        if (stk1.empty() || stk2.empty())
            return stk1.empty() ? stk2.top().second : stk1.top().second;
        else
            return ::min(stk1.top().second, stk2.top().second);
    }

    T front() {
        if (stk2.empty()) {
            while (!stk1.empty()) {
                T element = stk1.top().first;
                stk1.pop();

                T minimum = stk2.empty() ? element : ::min(element, stk2.top().second);
                stk2.emplace(element, minimum);
            }
        }
        return stk2.top().first;
    }

    void push(T x) {
        stk1.emplace(x, stk1.empty() ? x : ::min(x, stk1.top().second));
    }

    void pop() {
        if (stk2.empty()) {
            while (!stk1.empty()) {
                T element = stk1.top().first;
                stk1.pop();

                T minimum = stk2.empty() ? element : ::min(element, stk2.top().second);
                stk2.emplace(element, minimum);
            }
        }
        stk2.pop();
    }
};
