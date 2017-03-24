#include <time.h>

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     do { cerr << "elapsed time (" #i ") = " << (double(clock() - start##i) / CLOCKS_PER_SEC) << endl; } while (0)
