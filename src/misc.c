#include "cio.h"

#if defined(_WIN32)

static uint64_t hrtime_frequency_ = 0;

static void s_cio_hrtime_init_once(void)
{
    LARGE_INTEGER perf_frequency;
    if (QueryPerformanceFrequency(&perf_frequency) == 0)
    {
        abort();
    }

    hrtime_frequency_ = perf_frequency.QuadPart;
}

uint64_t cio_hrtime(void)
{
    static cio_once_t token = CIO_ONCE_INIT;
    cio_once(&token, s_cio_hrtime_init_once);

    LARGE_INTEGER counter;
    if (QueryPerformanceCounter(&counter) == 0)
    {
        abort();
    }

    double scaled_freq = (double)hrtime_frequency_ / 1000000000;
    double result = (double)counter.QuadPart / scaled_freq;
    return (uint64_t)result;
}

#else

uint64_t cio_hrtime(void)
{
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
    {
        abort();
    }
    return t.tv_sec * (uint64_t) 1e9 + t.tv_nsec;
}

#endif

void cio_shutdown_library(void)
{
}
