Testing gettimeofday and clock_gettime syscalls for appearence in vDSO ( https://man7.org/linux/man-pages/man7/vdso.7.html ). If strace ouput has no one fact of these syscalls appearence 
then they area implemented in vDSO.

To compile and run: ./vdso_test_script
