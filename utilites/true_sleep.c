#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#define TIME(name) ( name##_time = name##_sec +  (double) name##_nsec / nsecs_in_sec )

static unsigned int sleep_period = 10;

static const uint64_t nsecs_in_sec = 1e9;
static const uint64_t usecs_in_sec = 1e6;
static const uint64_t nsecs_in_usec = 1e3;

/**
 * sleep for `sec' seconds, without relying on the wall clock of time(2)
 * or gettimeofday(2).
 *
 * under ideal conditions is accurate to one microsecond. To get nanosecond
 * accuracy, replace sleep()/usleep() with something with higher resolution
 * like nanosleep() or ppoll().
 */
void true_sleep(int sec) {
        struct timespec ts_start;
        struct timespec ts_end;

        clock_gettime(CLOCK_MONOTONIC, &ts_start);

        ts_end = ts_start;
        ts_end.tv_sec += sec;

        for(;;) {
                struct timespec ts_current;
                struct timespec ts_remaining;

                clock_gettime(CLOCK_MONOTONIC, &ts_current);

                ts_remaining.tv_sec = ts_end.tv_sec - ts_current.tv_sec;
                ts_remaining.tv_nsec = ts_end.tv_nsec - ts_current.tv_nsec;
                while (ts_remaining.tv_nsec > nsecs_in_sec) {
                        ts_remaining.tv_sec++;
                        ts_remaining.tv_nsec -= nsecs_in_sec;
                }
                while (ts_remaining.tv_nsec < 0) {
                        ts_remaining.tv_sec--;
                        ts_remaining.tv_nsec += nsecs_in_sec;
                }

                if (ts_remaining.tv_sec < 0) {
                        break;
                }

                if (ts_remaining.tv_sec > 0) {
                        usleep(ts_remaining.tv_sec * usecs_in_sec);
                } else {
                        usleep(ts_remaining.tv_nsec / nsecs_in_usec);
                }
        }
}

int main(int argc, char * const argv[]) {

        if(argc > 1 && argv[1]) {
            int res = atoi(argv[1]);
            if(res > 0){
                sleep_period = res;
            }
        }	

        struct timespec tv_st;
        struct timespec tv_end;

        uint64_t est_sec = sleep_period;
        uint64_t est_nsec = 0;
        double est_time = 0;

        TIME(est);

        clock_gettime(CLOCK_MONOTONIC, &tv_st);

        true_sleep(sleep_period);

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

        printf("Delay of true_sleep(%u) = %f secs\n", sleep_period, diff_time  );
}
