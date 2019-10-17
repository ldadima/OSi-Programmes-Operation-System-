#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 10
pthread_mutexattr_t attr;
pthread_mutex_t thr[2];
int flag = 0;

void *thr_func(void *arg) {
    for (int i = 0; i < MAX;) {
        if (flag == 1) {
            pthread_mutex_lock(&thr[1]);
            write(1, "Thread 2\n", 9);
            flag--;
            pthread_mutex_unlock(&thr[1]);
            i++;
            pthread_mutex_lock(&thr[0]);
        }
        else {
           if(i!=0)
                pthread_mutex_unlock(&thr[0]);
        }
    }
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
    for (int i = 0; i < 2; i++) {
        err = pthread_mutex_init(&thr[i], NULL);
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

    for (int i = 0; i < MAX;) {
        if (flag == 0) {
            pthread_mutex_lock(&thr[1]);

            write(1, "THR 1\n", 6);
            flag++;
            pthread_mutex_unlock(&thr[1]);
            i++;
            pthread_mutex_lock(&thr[0]);
        }
        else {
            pthread_mutex_unlock(&thr[0]);
        }
    }

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

