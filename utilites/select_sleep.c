#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>  

#define TIME(name) ( name##_time = name##_sec +  (double) name##_nsec / nsecs_in_sec )

static uint64_t sleep_ns = 1e3;

const int64_t nsecs_in_sec = 1e9;

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression) \
  (__extension__                                                              \
    ({ long int __result;                                                     \
       do __result = (long int) (expression);                                 \
       while (__result == -1L && errno == EINTR);                             \
       __result; }))
#endif

int pselect_sleep (int filedes, uint64_t nsecs){
    fd_set set;
    struct timespec timeout;

    /* Initialize the file descriptor set. */
    FD_ZERO (&set);
    FD_SET (filedes, &set);

    /* Initialize the timeout data structure. */
    timeout.tv_sec = nsecs / nsecs_in_sec;
    timeout.tv_nsec = nsecs - timeout.tv_sec * nsecs_in_sec;

    /* pselect returns 0 if timeout, 1 if input available, -1 if error. */
    return pselect (FD_SETSIZE, &set, NULL, NULL, &timeout, NULL);
        /*return TEMP_FAILURE_RETRY (pselect (FD_SETSIZE,
                                     &set, NULL, NULL,
                                     &timeout, NULL));*/
}

int main (int argc, char * const argv[]) {

    if(argc > 1 && argv[1]) {
        long long res = atoll(argv[1]);
        if(res > 0){
            sleep_ns = res;
        }
    }	

    int pslct_res = 0;

    struct timespec tv_st;
    struct timespec tv_end;

    uint64_t est_sec = sleep_ns / nsecs_in_sec;
    uint64_t est_nsec = sleep_ns - est_sec * nsecs_in_sec;
    double est_time = 0;

    TIME(est);

    clock_gettime(CLOCK_MONOTONIC, &tv_st);

    pslct_res = pselect_sleep (STDIN_FILENO, sleep_ns);

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

    printf("Delay of pselect_sleep(%lu) = %2.9f secs\n", sleep_ns, diff_time  );
    
    fprintf (stderr, "pselect returned %d.\n", pslct_res);


  return 0;
}