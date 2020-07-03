#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#define TIME(name) ( name##_time = name##_sec +  (double) name##_nsec / NSECS_IN_SEC )

static uint64_t ns_sleep_period = 1e3;

#define CPU_FREQ 3600e6
#define NSECS_IN_SEC 1e9


const double cpu_ticks_in_ns = (CPU_FREQ / NSECS_IN_SEC);

/**
 * sleep for `nsec' nanoseconds by using NOP assembly instruction.
 */
inline void nop_sleep_ns(uint64_t nsec) {
        
        for(int i = 0, iters = cpu_ticks_in_ns * nsec; i < iters; i++){
                asm("nop");
        }

}

int main(int argc, char * const argv[]) {

        if(argc > 1 && argv[1]) {
            long long res = atoll(argv[1]);
            if(res > 0){
                ns_sleep_period = res;
            }
        }	

        struct timespec tv_st;
        struct timespec tv_end;

        uint64_t est_sec = ns_sleep_period / NSECS_IN_SEC;
        uint64_t est_nsec = ns_sleep_period - est_sec * NSECS_IN_SEC;
        double est_time = 0;

        TIME(est);

        clock_gettime(CLOCK_MONOTONIC, &tv_st);

        nop_sleep_ns(ns_sleep_period);

        uint64_t real_sec = 0;
        uint64_t real_nsec = 0;
        double real_time = 0;

        uint64_t diff_sec = 0;
        uint64_t diff_nsec = 0;
        double diff_time = 0;

        clock_gettime(CLOCK_MONOTONIC, &tv_end);

        real_sec = tv_end.tv_sec - tv_st.tv_sec;

        if(tv_end.tv_nsec < tv_st.tv_nsec){
            real_sec -= 1;
            real_nsec = tv_end.tv_nsec + (NSECS_IN_SEC - tv_st.tv_nsec);
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

        printf("Delay of nop_sleep_ns(%lu) = %ld nsecs\n", ns_sleep_period, (uint64_t) (diff_time * NSECS_IN_SEC) );
}
