/*
 * Copyright (c) 2003 Paul Herman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ONE_SECOND	1000000L

int count = 200;
int debug = 0;

int main (int ac, char **av) {
	long s;
	double diff;
	struct timespec tv1, tv2;

	if (ac > 1 && av[1])
		count = strtol(av[1], NULL, 10);

	while(count--) {
		clock_gettime(CLOCK_MONOTONIC, &tv1);
			/*
			 * Calculate the number of microseconds to sleep so we
			 * can wakeup right when the second hand hits zero.
			 *
			 * The latency for the following two statements is minimal.
			 * On a > 1.0GHz machine, the subtraction is done in a few
			 * nanoseconds, and the syscall to usleep/nanosleep is usualy
			 * less than 800 ns or 0.8 us.
			 */
		s = ONE_SECOND - tv1.tv_nsec / 1000;
		usleep(s);
		clock_gettime(CLOCK_MONOTONIC, &tv2);

		diff = (double)(tv2.tv_nsec - (tv1.tv_nsec + s * 1000))/1e9;
		diff += (double)(tv2.tv_sec - tv1.tv_sec);
		if (debug)
			printf("(%ld.%.9ld) ", tv2.tv_sec, tv2.tv_nsec);
		printf("%.9f\n", diff);
	}
	return 0;
}
