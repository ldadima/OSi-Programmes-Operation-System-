#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define num_steps 2000000
int num_threads;

char flag_exit=0;

//это 8ая лаба, для 7 закоментить signal() и раскоментить то что сейчас закоменчено в форе
void sig_exit(int arg){
	flag_exit = 1;
}

void *thr_func(void *arg) {
	/*if (signal(SIGINT, sig_exit) == SIG_ERR) {
		perror("Sig error\n");
		exit(1);
	}*/
	double pi = 0;
	//printf("%d\n", pthread_self());
	for (int i = 0; /*num_threads*i < num_steps*/; i++) {
		pi += 1.0 / ((num_threads*i+(pthread_self()-1))*4.0 + 1.0);
		pi -= 1.0 / ((num_threads*i + (pthread_self() - 1))*4.0 + 3.0);
		if ((i % 10000==0) && (flag_exit == 1)) {
			*(double*)arg = pi;
			pthread_exit((void*)0);
		}
	}
	*(double*)arg = pi;
	pthread_exit((void*)0);
}

int main(int argc, char** argv) {
	int err;
	void *stat;
	if (argv[1] == NULL) {
		perror("None arguments\n");
		exit(1);
	}
	num_threads = atoi(argv[1]);
	pthread_t *thrids = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
	double *pimass = (double*)malloc(sizeof(double)*num_threads);
	for (int i = 1; i < num_threads; i++) {
		err = pthread_create(&thrids[i], NULL, thr_func, &pimass[i]);
		if (err != 0) {
			perror("Thread creation error");
			exit(1);
		}
	}
	if (signal(SIGINT, sig_exit) == SIG_ERR) {
		perror("Sig error\n");
		exit(1);
	}

	double pi = 0;

	for (int i = 0; /*num_threads*i < num_steps*/; i++) {
		pi += 1.0 / ((num_threads*i)*4.0 + 1.0);
		pi -= 1.0 / ((num_threads*i)*4.0 + 3.0);
		if ((i % 10000 == 0) && (flag_exit == 1))
			break;
	}

	pi = pi;
	pimass[0] = pi;
	pi = 0;
	pi += pimass[0];
	for (int i = 1; i < num_threads;i++) {
		err=pthread_join(thrids[i],NULL);
		if (err != 0) {
			perror("Thread waiting error");
			exit(1);
		}
		pi += pimass[i];
	}
	printf("\npi = %.10f \n", pi*4);

	free(thrids);
	free(pimass);
	return (0);
}
