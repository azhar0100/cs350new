# This is a Makefile.

CC=gcc
CFLAGS=-Wall

all: imEnhance minidb

imEnhance: imEnhance.c iplib2New-modified.c
	$(CC) $(CFLAGS) -O3 -lm -o imEnhance         imEnhance.c
#	$(CC) $(CFLAGS) -pg -lm -o imEnhance.gprof.o imEnhance.c
#	^^ this can be used for profiling prior to hand-optimization
#	$(CC) $(CFLAGS) -g  -lm -o imEnhance.gprof.o imEnhance.c
#	^^ or you might need a debug build (ok, more likely, I'll need it.)

minidb: minidb.c
	$(CC) $(CFLAGS) -o minidb minidb.c

clean:
	rm -rf imEnhance imEnhance.gprof.o gmon.out test_output
	rm -rf minidb

test:
	rm -rf test_output
	./run_tests
