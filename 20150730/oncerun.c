/*************************************************************************
	> File Name: oncerun.c
	> Author: 
	> Mail: 
	> Created Time: Thu 30 Jul 2015 02:44:35 PM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>

pthread_once_t once = PTHREAD_ONCE_INIT;
void run()
{
    printf("fuction run is running in thread :%u\n",pthread_self());
}

void * thread1()
{
    pthread_t thid = pthread_self();
    printf("Current thread's ID is %u\n",thid);
    pthread_once(&once,run);
    printf("thread1 ends\n");
}
void * thread2()
{
    pthread_t thid = pthread_self();
    printf("Current thread's ID is %u\n",thid);
    pthread_once(&once,run);
    printf("thread2 ends\n");
}

int main(int argc,char *argv[])
{
    pthread_t thid1,thid2;
    
    pthread_create(&thid1,NULL,thread1,NULL);
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(3);
    printf("main thread exit\n");
    exit(0);
}
