#include <sys/time.h>
#include <stddef.h>
#include "mongoose_timer.h"

/*
 * 获取当前时间戳,精确到小数点秒
 */
double get_current_precision_time(void)
{
    struct timeval tv = {0,0};
    if(gettimeofday(&tv, NULL) != 0)
        return 0;
    double now = (double) tv.tv_sec + (((double) tv.tv_usec) / 1000000.0);
    return now;
}

