#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

static const uint64_t nsecs_in_sec = 1e9;
static const uint64_t iters = 1e6;

int main(int argc, char *argv[]){
	/*
        struct timeval tv;
        int i = 0;
        for (; i<100; i++) {
                gettimeofday(&tv,NULL);
        }
        */

        uint64_t real_sec = 0;
        uint64_t real_nsec = 0;
        double real_time = 0;

        struct timespec ts, tv_st, tv_end;

        clock_gettime(CLOCK_MONOTONIC, &tv_st);

        int j = 0;
        for (; j<iters; j++) {
                clock_gettime(CLOCK_MONOTONIC, &ts);
        }

        clock_gettime(CLOCK_MONOTONIC, &tv_end);

        real_sec = tv_end.tv_sec - tv_st.tv_sec;

        if(tv_end.tv_nsec < tv_st.tv_nsec){
            real_sec -= 1;
            real_nsec = tv_end.tv_nsec + (nsecs_in_sec - tv_st.tv_nsec);
        } else {
            real_nsec = tv_end.tv_nsec - tv_st.tv_nsec;
        }

        real_time = real_sec +  (double) real_nsec / nsecs_in_sec ;

        printf("Real time: %2.9f\n" , real_time );

        printf("Avg time latency of %ld calls to clock_gettime = %.0f nsecs\n", iters, real_time * nsecs_in_sec / iters  );


        return 0;
}
