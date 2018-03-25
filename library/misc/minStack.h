#pragma once

template <typename T>
struct MinStack {
    vector<T> stack;
    vector<T> stackMin;

    size_t size() const {
        return stack.size();
    }

    bool empty() const {
        return stack.empty();
    }

    T top() const {
        return stack.back();
    }

    T min() const {
        return stackMin.back();
    }

    void push(T x) {
        if (stack.empty())
            stackMin.push_back(x);
        else
            stackMin.push_back(::min(stackMin.back(), x));
        stack.push_back(x);
    }

    void pop() {
        stack.pop_back();
        stackMin.pop_back();
    }
};
