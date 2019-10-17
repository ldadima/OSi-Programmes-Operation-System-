#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX 10
pthread_mutexattr_t attr;
sem_t semaphore[2];

void *thr_func(void *arg) {
    for (int i = 0; i < MAX; i++) {
        sem_wait(&semaphore[1]);
        printf("Thread 2: string %d\n", i + 1);
        sem_post(&semaphore[0]);
    }
    pthread_exit((void *)0);
};


int main(int argc, char* argv[]) {
    int err;
    void *stat;
    pthread_t thr2;
    for (int i = 0; i < 2; i++) {
        err=sem_init(&semaphore[i], 0,1);
        if (err != 0) {
            perror("Semaphore initial error");
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
sem_wait(&semaphore[1]);
    for (int i = 0; i < MAX; i++) {
        sem_wait(&semaphore[0]);
        printf("THR 1: string %d\n", i + 1);
        sem_post(&semaphore[1]);
    }
    for (int i = 0; i < 2; i++) {
        sem_destroy(&semaphore[i]);
    }
    err = pthread_join(thr2, &stat);
    if (err != 0) {
        perror("Thread waiting error");
        exit(1);
    }
    return 0;
}

