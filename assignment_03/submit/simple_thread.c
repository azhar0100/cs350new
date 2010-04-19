#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define unless(x) if(!(x))

void * child(void *arg);
int get_thread_id(void);

/* Six threads... */
#define num_threads 6
pthread_t threads[num_threads];

/* ...will create a 6-character message for us. */
char message[8] = "      .";

int main(void) {
	int i;

	/* Create some threads that will go off and do some work, starting in the child() function. */
	for (i=0; i<num_threads; i++){
		unless( pthread_create(&threads[i], NULL, child, NULL) == 0 ){
			perror("pthread_create returned error");
			exit(-1); /* abnormal exit. */
		}
	}

	/* Wait in turn for all threads to complete execution. */
	for (i=0; i<num_threads; i++){
		unless( pthread_join(threads[i], NULL) == 0 ){
			perror("pthread_join returned error");
			exit(-2); /* abnormal exit. */
		}
	}

	/* Print out the message they worked together to author. */
	printf("%s\n", message);

	return 0; /* exit successfully. */
}

void * child(void *arg){ /* Start-of-execution for spawned threads. */
	int thread_id = get_thread_id();
	message[thread_id] = thread_id + 65;
	return NULL;
}

int get_thread_id(void){
	int i;
	int thread_id = -1;
	for (i=0; i<num_threads; i++) {
		if( pthread_equal(pthread_self(), threads[i]) ){
			thread_id = i;
			break;
		}
	}
	return thread_id;
}
