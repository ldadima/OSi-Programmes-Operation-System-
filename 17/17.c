#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


typedef struct list {
    char buf[81];
    struct list *next;
    pthread_mutex_t mutex;
} str_list;

str_list *head;
char exit_flag = 1;


void *thr_func(void *arg) {
    while (1) {
        sleep(5);
        str_list *tail = NULL;
        if (head != NULL) {
            str_list *prev = NULL;
            while (head->next != tail) {
                str_list *current = head;
                prev = NULL;
                while (current->next != tail) {
                    str_list *one;
                    if (strcmp(current->buf, current->next->buf) > 0) {
                        if (prev == NULL) {
                            pthread_mutex_t mutex[2] = {current->mutex, current->next->mutex};
                            pthread_mutex_lock(&mutex[0]);
                            pthread_mutex_lock(&mutex[1]);
                            head = current->next;
                            one = current->next->next;
                            current->next->next = current;
                            current->next = one;
                            pthread_mutex_unlock(&mutex[0]);
                            pthread_mutex_unlock(&mutex[1]);
                        } else {
                            pthread_mutex_t mutex[2] = {current->mutex, current->next->mutex};
                            pthread_mutex_lock(&(prev->mutex));
                            pthread_mutex_lock(&mutex[0]);
                            pthread_mutex_lock(&mutex[1]);
                            prev->next = current->next;
                            one = current->next->next;
                            current->next->next = current;
                            current->next = one;
                            pthread_mutex_unlock(&(prev->mutex));
                            pthread_mutex_unlock(&mutex[0]);
                            pthread_mutex_unlock(&mutex[1]);
                        }
                    } else {
                        current = current->next;
                    }
                    if (prev == NULL)
                        prev = head;
                    else
                        prev = prev->next;
                }
                tail = current;
            }
        }
    }
}

void sig_exit(int arg) {
    exit_flag = 0;
}


int main() {
    head = NULL;
    int err;
    pthread_t thr;

    if (signal(SIGINT, sig_exit) == SIG_ERR) {
        perror("Sig error\n");
        exit(1);
    }


    err = pthread_create(&thr, NULL, thr_func, NULL);
    if (err != 0) {
        perror("Thread creation error");
        exit(1);
    }

    while (exit_flag) {
        char buf[81];
        if (fgets(buf, 81, stdin) != NULL) {
            int end=strcspn(buf, "\n");
            buf[end] = '\0';
            if (strlen(buf) > 0) {
                str_list *cur = (str_list *) malloc(sizeof(str_list));
                err = pthread_mutex_init(&cur->mutex, NULL);
                if (err != 0) {
                    perror("Mutex initial error");
                    exit(1);
                }
                strcpy(cur->buf, buf);
                cur->next=head;
                if(head!=NULL){
                pthread_mutex_t mutex = head->mutex;
                pthread_mutex_lock(&mutex);
                pthread_mutex_lock(&cur->mutex);
                head=cur;
                pthread_mutex_unlock(&mutex);
                pthread_mutex_unlock(&cur->mutex);
                }
                else
                    head=cur;
            } else {
                if (head != NULL) {
                    str_list *one = head;
                    printf("List: \n");
                    while (one != NULL) {
                        pthread_mutex_lock(&one->mutex);
                        printf("%s\n", one->buf);
                        pthread_mutex_unlock(&one->mutex);
                        one = one->next;
                    }
                    printf("End\n");
                } else
                    printf("List Empty\n");
            }
        }
    }

    err = pthread_cancel(thr);
    if (err != 0) {
        perror("Thread cancel error");
    }
    err = pthread_join(thr, NULL);
    if (err != 0) {
        perror("Thread waiting error");
    }
    str_list *one = head;
    while (head != NULL) {
        head = head->next;
        pthread_mutex_destroy(&one->mutex);
        free(one);
        one = head;
    }
    return 0;
}
