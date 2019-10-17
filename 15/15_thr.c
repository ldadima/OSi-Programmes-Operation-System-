#include "15.h"

int main() {
    sem_t *sem1;
    sem_t *sem2;
    if ((sem1 = sem_open("/my_sem1", O_EXCL)) == SEM_FAILED) {
        perror("Sem Create Error");
        return -1;
    }
    if ((sem2 = sem_open("/my_sem2", O_EXCL)) == SEM_FAILED) {
        perror("Sem Create Error");
        return -1;
    }
    for (int i = 0; i < MAX; i++) {
        sem_wait(sem2);
        printf("PROC 2\n");
        sem_post(sem1);
    }
    sem_close(sem1);
    sem_close(sem2);
    return 0;
}
