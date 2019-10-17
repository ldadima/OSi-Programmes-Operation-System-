#include "15.h"

int main() {
    sem_t *sem1;
    sem_t *sem2;
    if ((sem1 = sem_open("/my_sem1", O_CREAT, 0600, 1)) == SEM_FAILED) {
        perror("Sem Create Error");
        return -1;
    }
    if ((sem2 = sem_open("/my_sem2", O_CREAT, 0600, 1)) == SEM_FAILED) {
        perror("Sem Create Error");
        return -1;
    }
    pid_t proc;
    if ((proc = fork()) == 0) {
        execl("15_thr", "15_thr", (char *) 0);
        return 0;
    }
    sem_wait(sem2);
    for (int i = 0; i<MAX; i++) {
        sem_wait(sem1);
        printf("PROC 1\n");
        sem_post(sem2);
    }
    int *procret;
    wait(procret);
    if(*procret!=0){
        perror("Error new proc");
    }
    sem_close(sem1);
    sem_close(sem2);
    sem_unlink("/my_sem1");
    sem_unlink("/my_sem2");
    return 0;
}
