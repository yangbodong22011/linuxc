/*************************************************************************
	> File Name: cat_threadPrival.c
	> Author: 
	> Mail: 
	> Created Time: Mon 03 Aug 2015 09:38:05 PM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
void *func();

void *func()
{
    sleep(10);
    pthread_exit(0);
}

int main(int argc,char *argv[])
{
    int* status;
    pthread_t  th1;
    pthread_t  th2;
    pthread_t  th3;
    pid_t      pid;
    printf("the pid is:%d\n",getpid());
    printf("now is the 1 thread\n");
    pthread_create(&th1,NULL,func,NULL);
    pthread_join(th1,(void**)&status);
    printf("now is the 2 thread\n");
    pthread_create(&th2,NULL,func,NULL);
    pthread_join(th2,(void**)&status);
    printf("now is the 3 thread\n");
    pthread_create(&th3,NULL,func,NULL);
    pthread_join(th3,(void**)&status);


}
