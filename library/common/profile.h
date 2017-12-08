#include <time.h>
#include <chrono>

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     do { cerr << "elapsed time (" #i ") = " << (double(clock() - start##i) / CLOCKS_PER_SEC) << endl; } while (0)

#define PROFILE_HI_START(i) auto start##i = chrono::high_resolution_clock::now()
#define PROFILE_HI_STOP(i)  do { cerr << "elapsed time (" #i ") = " \
                                      << (chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start##i).count() / 1000000.0) \
                                      << endl; } while (0)

struct AccumulateTimer {
    AccumulateTimer() {
        time = 0;
    }

    void reset() {
        time = 0;
    }

    void start() {
        beg = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - beg).count();
    }

    double getSec() const {
        return time / 1.0e9;
    }

    double getMillisec() const {
        return time / 1.0e6;
    }

    double getMicrosec() const {
        return time / 1.0e3;
    }

    long long getNanosec() const {
        return time;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> beg;
    long long time;
};


struct AccumulateValue {
    AccumulateValue() {
        sum = 0;
        cnt = 0;
    }

    void reset() {
        sum = 0;
        cnt = 0;
    }

    void add(int val) {
        sum += val;
        cnt++;
    }

    long long getSum() const {
        return sum;
    }

    long long getCount() const {
        return cnt;
    }

    double getAverage() const {
        return cnt == 0 ? 0.0 : double(sum) / cnt;
    }

private:
    long long sum;
    long long cnt;
};
