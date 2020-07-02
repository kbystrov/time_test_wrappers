#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

static unsigned int iters = 10;
static unsigned int sleep_period = 1;
static const uint64_t nsecs_in_sec = 1e9;

#define TIME(name) ( name##_time = name##_sec +  (double) name##_nsec / nsecs_in_sec )

int main(int argc, char * const argv[]){

        if(argc > 1 && argv[1]) {
            int res = atoi(argv[1]);
            if(res > 0){
                sleep_period = res;
            }
        }	

        if (argc > 2 && argv[2]) {
            int res =  atoi(argv[2]);
            if(res > 0){
                iters = res;
            }   
        }	

        struct timespec tv_st, tv_end;

        unsigned int est_sec = sleep_period * iters;
        unsigned int est_nsec = 0;
        double est_time = 0;

        TIME(est);

        unsigned int real_sec = 0;
        unsigned int real_nsec = 0;
        double real_time = 0;

        unsigned int diff_sec = 0;
        unsigned int diff_nsec = 0;
        double diff_time = 0;

        clock_gettime(CLOCK_MONOTONIC, &tv_st);
    
        for (unsigned int i = 0; i < iters; i++) {
            sleep(sleep_period);
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

        printf("Estimated time: %.9f\n", est_time);
        printf("Real time: %.9f\n" , real_time );
        printf("Diff time: %.9f\n" , diff_time );

        printf("Avg time latency %u of sleep(%u) = %.9f secs\n", iters, sleep_period, diff_time  );

        return 0;
}
