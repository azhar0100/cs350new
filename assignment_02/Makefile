imEnhance: imEnhance.c imageInfo/iplib2New.c
	gcc -Wall -O3 -o imEnhance imEnhance.c -lm
	gcc -Wall -pg -o imEnhance.gprof.o imEnhance.c -lm

clean:
	rm -rf imEnhance imEnhance.gprof.o gmon.out test_output

test:
	./run_tests
