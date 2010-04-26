#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define unless(x) if(!(x))

void * child(void *arg);
int get_thread_id(void);

/* Six threads... */
#define NUM_THREADS 6
pthread_t threads[NUM_THREADS];

/* Type thread_args_t */
typedef struct {
	int thread_number;	/* 0 .. NUM_THREADS		*/
	char letter;		/* Each thread contributes 1 letter to the message.	*/

	int retval;		/* Set by the child threads.	*/
} thread_args_t;

/* Array of thread_args_t's, one for each child thread. */
thread_args_t thread_args[NUM_THREADS];

/* ...will create a 6-character message for us. */
char message[8] = "      .";

int main(void) {
	int i;

	/* Create some threads that will go off and do some work, starting in the child() function. */
	for (i=0; i<NUM_THREADS; i++){

		thread_args[i].thread_number = i;
		thread_args[i].letter = 65 + i;

		unless( pthread_create(&threads[i], NULL, child, (void *)&thread_args[i]) == 0 ){
			perror("pthread_create returned error");
			exit(-1); /* abnormal exit. */
		}
	}

	/* Wait in turn for all threads to complete execution. */
	void *result;
	for (i=0; i<NUM_THREADS; i++){
		unless( pthread_join(threads[i], &result) == 0 ){
			perror("pthread_join returned error");
			exit(-2); /* abnormal exit. */
		}

		printf("Child thread #%d exited with return value %d.\n", i, *(int *)result);
	}

	/* Print out the message they worked together to author. */
	printf("Message is: %s\n", message);

	return 0; /* exit successfully. */
}

void * child(void *arg){ /* Start-of-execution for spawned threads. */
	thread_args_t *my_args = (thread_args_t *)arg;
	message[my_args->thread_number] = my_args->letter;
	my_args->retval = 100 * my_args->thread_number + 100;
	pthread_exit((void *)&(my_args->retval));
}
