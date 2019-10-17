#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 10
pthread_mutexattr_t attr;
pthread_mutex_t thr;
pthread_cond_t flag;

void *thr_func(void *arg) {
    pthread_cond_signal(&flag);
    for (int i = 0; i < MAX;i++) {
        pthread_cond_wait(&flag,&thr);
        printf("Thread 2\n");
        pthread_cond_signal(&flag);
    }
    pthread_mutex_unlock(&thr);
    pthread_exit((void *) 0);
};


int main(int argc, char *argv[]) {
    int err;
    void *stat;
    pthread_t thr2;
    if (pthread_mutexattr_init(&attr) != 0) {
        perror("Attr initial error");
        exit(-1);
    }
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) != 0) {
        perror("Set type error");
        exit(-1);
    }
    err = pthread_mutex_init(&thr, NULL);
    if (err != 0) {
        perror("Mutex initial error");
        exit(1);
    }
    err = pthread_cond_init(&flag,NULL);
    if (err != 0) {
        perror("Cond_var initial error");
        exit(1);
    }
    err = pthread_create(&thr2, NULL, thr_func, NULL);
    if (err != 0) {
        perror("Thread creation error");
        for (int i = 0; i < 2; i++)
            err = pthread_mutex_destroy(&thr);
        exit(1);
    }
    pthread_mutex_lock(&thr);
    for (int i = 0; i < MAX;i++) {
        pthread_cond_wait(&flag,&thr);
        printf("THR 1\n");
        pthread_cond_signal(&flag);
    }
    pthread_mutex_unlock(&thr);
    pthread_cond_destroy(&flag);
    pthread_mutex_destroy(&thr);
    pthread_mutexattr_destroy(&attr);

    err = pthread_join(thr2, &stat);
    if (err != 0) {
        perror("Thread waiting error");
        exit(1);
    }

    return 0;
}

