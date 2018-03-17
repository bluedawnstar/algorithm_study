#include <random>
#include <limits>

struct RandInt64 {
    static long long get() {
        //static std::random_device rd;
        //static std::mt19937_64 eng(rd());
        static std::mt19937_64 eng(1);
        static std::uniform_int_distribution<long long> dist(0, std::numeric_limits<long long>::max());
        return dist(eng);
    }
};

struct RandUInt64 {
    static unsigned long long get() {
        //static std::random_device rd;
        //static std::mt19937_64 eng(rd());
        static std::mt19937_64 eng(1);
        static std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());
        return dist(eng);
    }
};

struct RandInt32 {
    static int get() {
        //static std::random_device rd;
        //static std::mt19937 eng(rd());
        static std::mt19937 eng(1);
        static std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
        return dist(eng);
    }
};

struct RandUInt32 {
    static unsigned int get() {
        //static std::random_device rd;
        //static std::mt19937 eng(rd());
        static std::mt19937 eng(1);
        static std::uniform_int_distribution<unsigned int> dist(0, std::numeric_limits<unsigned int>::max());
        return dist(eng);
    }
};
