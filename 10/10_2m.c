#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX 10
#define TIME 2
pthread_mutexattr_t attr;
pthread_mutex_t thr[2];


void *thr_func(void *arg) {
	pthread_mutex_lock(&thr[1]);
	for (int i = 0; i < MAX*2*TIME; i++) {
		if(i%(2*TIME)==3)
			write(STDOUT_FILENO,"Thr 2\n",strlen("THR 1\n"));
		if(i%TIME==0){
		pthread_mutex_unlock(&thr[(i%TIME+1)%2]);
		pthread_mutex_lock(&thr[(i%TIME)%2]);}
	}
	pthread_mutex_unlock(&thr[1]);
	pthread_exit((void *)0);
};


int main(int argc, char* argv[]) {
	int err;
	void *stat;
	pthread_t thr2;
	/*if (pthread_mutexattr_init(&attr) != 0) {
		perror("Attr initial error");
		exit(-1);
	}
	if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) != 0) {
		perror("Set type error");
		exit(-1);
	}*/
	for (int i = 0; i < 2; i++) {
		err=pthread_mutex_init(&thr[i], NULL);
		if (err != 0) {
			perror("Mutex initial error");
			exit(1);
		}
	}
	err = pthread_create(&thr2, NULL, thr_func, NULL);
	if (err != 0) {
		perror("Thread creation error");
		for (int i = 0; i < 2; i++) 
			err = pthread_mutex_destroy(&thr[i]);
		exit(1);
	}
	pthread_mutex_lock(&thr[0]);
		
	for (int i = 0; i < MAX*2*TIME; i++) {
		if(i%(2*TIME)==0)
			write(STDOUT_FILENO,"THR 1\n",strlen("THR 1\n"));

		if(i%TIME==0){
		pthread_mutex_unlock(&thr[(i%TIME)%2]);
		pthread_mutex_lock(&thr[(i%TIME+1)%2]);}
	}
	pthread_mutex_unlock(&thr[0]);
	for (int i = 0; i < 2; i++) {
		pthread_mutex_destroy(&thr[i]);
	}
	pthread_mutexattr_destroy(&attr);
	err = pthread_join(thr2, &stat);
	if (err != 0) {
		perror("Thread waiting error");
		exit(1);
	}
	return 0;
}

