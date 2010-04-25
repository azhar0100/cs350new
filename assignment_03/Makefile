# This is a Makefile.

CC=gcc
CFLAGS=-Wall

all: pimEnhance timEnhance simple_thread simple_thread_pass_an_arg

simple_thread: simple_thread.c 
	$(CC) $(CFLAGS) -lpthread -D_REENTRANT -o simple_thread simple_thread.c

simple_thread_pass_an_arg: simple_thread_pass_an_arg.c 
	$(CC) $(CFLAGS) -lpthread -D_REENTRANT -o simple_thread_pass_an_arg simple_thread_pass_an_arg.c

timEnhance: timEnhance.c iplib2New-modified.c im_shared.h
	$(CC) $(CFLAGS) -O3 -lm -lpthread -D_REENTRANT -o timEnhance         timEnhance.c

pimEnhance: pimEnhance.c iplib2New-modified.c im_shared.h
	$(CC) $(CFLAGS) -O3 -lm -o pimEnhance         pimEnhance.c

#	$(CC) $(CFLAGS) -O3 -lm -o imEnhance         imEnhance.c
#	$(CC) $(CFLAGS) -pg -lm -o imEnhance.gprof.o imEnhance.c
#	^^ this can be used for profiling prior to hand-optimization
#	$(CC) $(CFLAGS) -g  -lm -o imEnhance.gprof.o imEnhance.c
#	^^ or you might need a debug build (ok, more likely, I'll need it.)

clean:
	rm -rf timEnhance pimEnhance test_output test_output.txt simple_thread simple_thread_pass_an_arg

test: all
	rm -rf test_output
	echo "See test_output.txt for output."
	./run_tests.sh 2>&1 | tee test_output.txt
