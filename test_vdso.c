#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int
main(int argc, char *argv[])
{
        struct timeval tv;
        volatile int i = 0;
        for (; i<100; i++) {
                gettimeofday(&tv,NULL);
        }
        
        struct timespec ts;									
        volatile int j = 0;
        for (; j<100; j++) {
                clock_gettime(CLOCK_MONOTONIC, &ts);
        }


        return 0;
}
