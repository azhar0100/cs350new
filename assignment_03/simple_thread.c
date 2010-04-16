#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void child(int arg);
void get_thread_id(void);

#define unless(x) if(!(x))

#define num_threads 5


pthread_t threads[num_threads];
char *message = "          .";

int main(void) {
	int i;

	for (i=0; i<num_threads; i++){
		unless( pthread_create(&threads[i], NULL, (void *)child, NULL) == 0 ){
		/* if(pthread_create(&children[i], NULL, (void *)&iterate_input_image, (void *)&args)){ ..error.. } */

			perror("pthread_create returned error");
			exit(-1); /* abnormal exit. */
		}
	}

	for (i=0; i<num_threads; i++){
		unless( pthread_join(threads[i], NULL) == 0 ){
			perror("pthread_join returned error");
			exit(-2); /* abnormal exit. */
		}
	}

	printf("%s\n", message);

	return 0; /* exit successfully. */
}

void child(int arg){
	for (arg=0; arg<num_threads; arg++) {
		if( pthread_equal(pthread_self(), threads[arg]) ){
			break;
			printf("did it.\n");
		}
	}
	message[arg] = 65+arg;
}

void get_thread_id(void){
/*	auto int i;
	auto int thread_id = -1;

			thread_id = i;
			break;
		}
	}

	return thread_id; */ 
}
