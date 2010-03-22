imEnhance: imEnhance.c imageInfo/iplib2New.c
	gcc -Wall -O3 -o imEnhance imEnhance.c -lm

gprof: imEnhance.c imageInfo/iplib2New.c
	gcc -Wall -pg -o imEnhance.gprof.o imEnhance.c -lm

clean:
	rm -rf out imEnhance

test:
	rm -rf out; mkdir out
	./run_tests
