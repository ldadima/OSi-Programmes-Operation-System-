#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

pthread_mutex_t mutex;

typedef struct list {
    char buf[81];
    struct list *next;
} str_list;

str_list *head = NULL;
char exit_flag = 1;


void *thr_func(void *arg) {
    while (1) {
        sleep(5);
        str_list *tail = NULL;
        pthread_mutex_lock(&mutex);
        if (head != NULL) {
            str_list *prev = NULL;
            while (head->next != tail) {
                str_list *current = head;
                prev = NULL;
                while (current->next != tail) {
                    str_list *one;
                    if (strcmp(current->buf, current->next->buf) > 0) {
                        if (prev == NULL) {
                            head = current->next;
                            one = current->next->next;
                            current->next->next = current;
                            current->next = one;
                        } else {
                            prev->next = current->next;
                            one = current->next->next;
                            current->next->next = current;
                            current->next = one;
                        }
                    } else{
                        current=current->next;
                    }
                    if(prev==NULL)
                        prev=head;
                    else
                        prev=prev->next;
                }
                tail = current;
            }
        }
        pthread_mutex_unlock(&mutex);
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

    err = pthread_mutex_init(&mutex, NULL);
    if (err != 0) {
        perror("Mutex initial error");
        exit(1);
    }

    err = pthread_create(&thr, NULL, thr_func, NULL);
    if (err != 0) {
        perror("Thread creation error");
        pthread_mutex_destroy(&mutex);
        exit(1);
    }

    while (exit_flag) {
        char buf[81];
        if (gets(buf) != NULL) {
            if (strlen(buf) > 0) {
                str_list *cur = (str_list *) malloc(sizeof(str_list));
                cur->next=head;
                strcpy(cur->buf, buf);
                pthread_mutex_lock(&mutex);
                head=cur;
                pthread_mutex_unlock(&mutex);
            } else {
                pthread_mutex_lock(&mutex);
                if (head != NULL) {
                    str_list *one = head;
                    printf("List: \n");
                    while (one != NULL) {
                        printf("%s\n", one->buf);
                        one = one->next;
                    }
                    printf("End\n");
                } else
                    printf("List Empty\n");
                pthread_mutex_unlock(&mutex);
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
    pthread_mutex_destroy(&mutex);
    str_list *one = head;
    while (head != NULL) {
        head = head->next;
        free(one);
        one = head;
    }
    return 0;
}
