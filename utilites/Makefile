CC=gcc
CFLAGS=-O2 -Wall -Wextra -Winline

SOURCES = nop_sleep.c select_sleep.c true_sleep.c
OBJECTS=$(SOURCES:.c=.o)

all: $(OBJECTS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o
