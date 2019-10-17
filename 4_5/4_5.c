#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void thr_end(void* arg) {
	int thr_num = *(int*)arg;
	printf("It's my end, thr %d :(\n",thr_num);
	return;
}

void *thr_func(void *arg) {
	int num_thread=pthread_self();
	pthread_cleanup_push(thr_end, &num_thread);
	//https://code.woboq.org/qt5/include/pthread.h.html
int i;
	while (i<10) {
		if (write(1, "Hello\n", 6) != 6)
			perror("Write Error");
i++;
	}
	pthread_cleanup_pop(1);
	printf("Check\n");
	pthread_exit((void*)0);
}

int main(int argc, char*argv[]) {
	int err;
	void *stat;
	pthread_t thrid;

	err = pthread_create(&thrid, NULL, thr_func, NULL);
	if (err != 0) {
		perror("Thread creation error");
	}
	
	sleep(1);
	printf("Cancel in progress...\n", thrid);
	sleep(1);

	err=pthread_cancel(thrid);
	if (err != 0) {
		perror("Thread cancel error");
	}
	
	printf("Thread %d is canceled\n", thrid);
	err = pthread_join(thrid, &stat); 
	if (err != 0) {
		perror("Thread waiting error");
	}
	printf("END OF WORK\n");
	return 0;
}
