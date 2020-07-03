Test for measuring latency of sleep utilities.

Compile: make all

Clean: make clean

Run test: test_{sleep_function} [period] [iters] - run $(iters) number of specified sleep for $(period) quannts of time (ns - for nanolseep(); us -for usleep(); etc.).

{sleep_function}: 
	- nanosleep;
	- usleep;
	- sleep;
	- clock_nanosleep; 

$(iters) and $(period) are optional parameters and have default values.
