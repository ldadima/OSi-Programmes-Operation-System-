#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>



int main(int argc, char* argv[]) {
	sem_t sem;
    int err;
    void *stat;
    pthread_t thr2;
    for (int i = 0; ; i++) {
        err=sem_init(&sem, 0,1);
        if (err != 0) {
		printf("%d\n",i+1);
            perror("Semaphore initial error");
            exit(1);
        }
    }

return 0;
}

