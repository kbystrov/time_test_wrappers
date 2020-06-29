Copyright (C) 2005, Ingo Molnar

Check TSC synchronity on x86 CPUs. Also detects gettimeofday() and clock_gettime() -level time warps.

To choose required time source (TSC, gettimeofday(), clock_gettime() ) set according macros (TEST_TSC, TEST_TOD, TEST_CLOCK) to 1.

To get resolution set TEST_GETRES to 1.

Compile: ./compile.sh

Run: ./time-wrap-test.o [number of processes]; Default naumber of processes - is maximum available on the current machine. 
