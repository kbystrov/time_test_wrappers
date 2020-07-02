#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

static const uint64_t iters = 1e3;
static const uint64_t sleep_period = 1e8;
static const uint64_t nsecs_in_sec = 1e9;

#define TIME(name) ( name##_time = name##_sec +  (double) name##_nsec / nsecs_in_sec )

int main(){		

        struct timespec tv, tv_st, tv_end;
        tv.tv_nsec = sleep_period;
        tv.tv_sec = 0;

        uint64_t est_sec = sleep_period * iters / nsecs_in_sec;
        uint64_t est_nsec = sleep_period * iters - est_sec * nsecs_in_sec;
        double est_time = 0;

        TIME(est);

        uint64_t real_sec = 0;
        uint64_t real_nsec = 0;
        double real_time = 0;

        uint64_t diff_sec = 0;
        uint64_t diff_nsec = 0;
        double diff_time = 0;

        clock_gettime(CLOCK_MONOTONIC, &tv_st);
    
        for (uint64_t i = 0; i < iters; i++) {
                nanosleep(&tv, NULL);
                //clock_nanosleep(CLOCK_MONOTONIC, 0, &tv, NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &tv_end);

        real_sec = tv_end.tv_sec - tv_st.tv_sec;

        if(tv_end.tv_nsec < tv_st.tv_nsec){
            real_sec -= 1;
            real_nsec = tv_end.tv_nsec + (nsecs_in_sec - tv_st.tv_nsec);
        } else {
            real_nsec = tv_end.tv_nsec - tv_st.tv_nsec;
        }

        TIME(real);

        diff_sec = real_sec - est_sec;
        diff_nsec = real_nsec - est_nsec;

        TIME(diff);

        printf("Estimated time: %2.9f\n", est_time);
        printf("Real time: %2.9f\n" , real_time );
        printf("Diff time: %2.9f\n" , diff_time );

        printf("Avg time latency %ld of nanosleep(%ld) = %.0f nsecs\n", iters, sleep_period, diff_time * nsecs_in_sec / iters  );

        return 0;
}
