#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

static uint64_t iters = 1e2;
static uint64_t sleep_period = 1e2;

static const uint64_t nsecs_in_sec = 1e9;
static const uint64_t usecs_in_sec = 1e6;
static const uint64_t nsecs_in_usec = 1e3;

#define TIME(name) ( name##_time = name##_sec +  (double) name##_usec / usecs_in_sec )

int main(int argc, char * const argv[]){

        if(argc > 1 && argv[1]) {
            int64_t res = atoll(argv[1]);
            if(res > 0){
                sleep_period = res;
            }
        }	

        if (argc > 2 && argv[2]) {
            int64_t res =  atoll(argv[2]);
            if(res > 0){
                iters = res;
            }   
        }	

        struct timespec tv_st, tv_end;

        uint64_t est_sec = sleep_period * iters / usecs_in_sec;
        uint64_t est_usec = sleep_period * iters - est_sec * usecs_in_sec;
        double est_time = 0;

        TIME(est);

        uint64_t real_sec = 0;
        uint64_t real_usec = 0;
        double real_time = 0;

        uint64_t diff_sec = 0;
        uint64_t diff_usec = 0;
        double diff_time = 0;

        clock_gettime(CLOCK_MONOTONIC, &tv_st);
    
        for (uint64_t i = 0; i < iters; i++) {
                usleep(sleep_period);
        }

        clock_gettime(CLOCK_MONOTONIC, &tv_end);

        real_sec = tv_end.tv_sec - tv_st.tv_sec;

        if(tv_end.tv_nsec < tv_st.tv_nsec){
            real_sec -= 1;
            real_usec = ( tv_end.tv_nsec + (nsecs_in_sec - tv_st.tv_nsec) ) / nsecs_in_usec;
        } else {
            real_usec = ( tv_end.tv_nsec - tv_st.tv_nsec ) / nsecs_in_usec;
        }

        TIME(real);

        diff_sec = real_sec - est_sec;
        diff_usec = real_usec - est_usec;

        TIME(diff);

        printf("Estimated time: %2.6f\n", est_time);
        printf("Real time: %2.6f\n" , real_time );
        printf("Diff time: %2.6f\n" , diff_time );

        printf("Avg time latency %ld of usleep(%ld) = %.0f usecs\n", iters, sleep_period, diff_time * usecs_in_sec / iters  );

        return 0;
}
