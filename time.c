#include <sys/time.h>
#include <unistd.h>

long get_timestamp_ms(long start_time)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time);
}