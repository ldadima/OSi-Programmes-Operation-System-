#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

void int2str(int i,char*buf){
    int c=i;
    int a=1;
    buf[0]='/';
    while(c>0){
        buf[a]=c%10+'0';
        c=c/10;
        a++;
    }
}


int main(int argc, char* argv[]) {
	sem_t *sem[65000];
    int err;
    void *stat;
    pthread_t thr2;
    int i;
    for (i=0; i<65000; i++) {
        char buf[256];
        int2str(i,buf);
        if ((sem[i]=sem_open(buf,O_CREAT,0600,1)) == SEM_FAILED) {
            printf("%d\n",i+1);
            perror("Sem Create Error");
            break;
        }
    }
    for (int j=0; j<62193; j++) {
        char buf[256];
        int2str(j,buf);
        sem_close(sem[j]);
        sem_unlink(buf);
    }
return 0;
}
