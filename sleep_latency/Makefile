CC=gcc
CFLAGS=-O2 -Wall -Wextra

SOURCES = test_nanosleep.c test_usleep.c test_sleep.c test_clock_nanosleep.c
OBJECTS=$(SOURCES:.c=.o)

all: $(OBJECTS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o
