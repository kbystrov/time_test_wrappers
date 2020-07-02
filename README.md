A bunch of tests for measuring time latency in Linux for my own needs.

clocks/ - test clocks (REAL, MONOTONIC, etc.) and gettimeofday for time resolution, min, max, avg of time distortion.

gettime_test/ - another group of tests for measuring time distorton.

ptpd/ - ptpd2 examples of config file and log.

vdso/ - test for vDSO availabiliy for using gettimeofday() and clock_gettime(). 

utilities/ - functions for work with time
