#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THR_NUM 4

void print(int num_thread) {
	printf("Hello, it's thread %d\n", num_thread);
	printf("Bye, it was thread %d,\n", num_thread);
	return;
}

void *thr_func(void *arg) {
	print((int)arg);
	pthread_exit((void*)0);
} 

int main(int argc, char*argv[]) {
	int err;
	void *stat;
	pthread_t thrid[THR_NUM];
	int i = 0;
	while(i++<THR_NUM) {
		err = pthread_create(&thrid[i], NULL, thr_func, (void*)i);
		if (err != 0) {
			perror("Thread creation error");
		}
	}
	i = 0;
	while (i++ < THR_NUM) {
		err = pthread_join(thrid[i], &stat);
		if (err != 0) {
			perror("Thread waiting error");
		}
	}
	printf("End of work\n");
	return 0;
}
