#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

static const uint64_t iters = 2000;
static const uint64_t sleep_period = 1e3;
static const uint64_t nsecs_in_sec = 1e9;
static const uint64_t nsec_in_usec = 1e3;

int main(){		
    /*						
        for (int i = 0; i < 2000; i++) {
                usleep(100);
        }
        */

        struct timespec tv, tv_st, tv_end;
        tv.tv_nsec = sleep_period;
        tv.tv_sec = 0;

        uint64_t est_sec = sleep_period * iters / nsecs_in_sec;
        uint64_t est_nsec = sleep_period * iters - est_sec * nsecs_in_sec;

        uint64_t real_sec = 0;
        uint64_t real_nsec = 0;

        uint64_t diff_sec = 0;
        uint64_t diff_nsec = 0;

        clock_gettime(CLOCK_MONOTONIC, &tv_st);
    
        for (uint64_t i = 0; i < iters; i++) {
                nanosleep(&tv, NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &tv_end);

        real_sec = tv_end.tv_sec - tv_st.tv_sec;

        if(tv_end.tv_nsec < tv_st.tv_nsec){
            real_sec -= 1;
            real_nsec = tv_end.tv_nsec + (nsecs_in_sec - tv_st.tv_nsec);
        } else {
            real_nsec = tv_end.tv_nsec - tv_st.tv_nsec;
        }

        diff_sec = real_sec - est_sec;
        diff_nsec = real_nsec - est_nsec;

        printf("Estimated time: %ld.%ld\n", est_sec, est_nsec);
        printf("Real time: %ld.%ld\n" , real_sec, real_nsec );
        printf("Diff time: %ld.%ld\n" , diff_sec, diff_nsec );

        printf("Avg time latency %ld of nanosleep(%ld) = %ld usecs\n", iters, sleep_period, (diff_sec * nsecs_in_sec + diff_nsec) / iters / nsec_in_usec );

        return 0;
}
