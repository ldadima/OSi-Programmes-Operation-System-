#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MUTEX_COUNT 3
#define ITERATIONS 10

pthread_mutex_t m[MUTEX_COUNT];
pthread_mutexattr_t attr;

void *print_message(void *str) {
    int i = 0, k = 1;
    if(strcmp("Thread 2\n",(char*)str)==0){
        pthread_mutex_lock(&m[0]);
        pthread_mutex_lock(&m[2]);
    }
    pthread_mutex_unlock(&m[0]);
    for (i = 0; i < ITERATIONS * MUTEX_COUNT; i++) {
        pthread_mutex_lock(&m[k]);
        k = (k + 1) % 3;
        pthread_mutex_unlock(&m[k]);
        if (k == 2) {
            write(STDOUT_FILENO, str, strlen(str));
        }
        k = (k + 1) % 3;
    }
    pthread_mutex_unlock(&m[2]);
    pthread_exit(NULL);
}

int main() {
    pthread_t pthread;
    int i = 0;

    if (pthread_mutexattr_init(&attr) != 0) {
        perror("Attr initial error");
        pthread_exit(NULL);
    }
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) != 0) {
        perror("Set type error");
        pthread_exit(NULL);
    }

    for (i = 0; i < MUTEX_COUNT; i++)
        pthread_mutex_init(&m[i], NULL);

    pthread_mutex_lock(&m[2]);
    if (pthread_create(&pthread, NULL, print_message, "Thread 2\n") < 0) {
        fprintf(stderr, "Error in creating thread!\n");
        pthread_exit(NULL);
    }

    print_message("THR 1\n");

    pthread_join(pthread, NULL);
    for (i = 0; i < MUTEX_COUNT; i++)
        pthread_mutex_destroy(&m[i]);

    pthread_exit(NULL);
}
