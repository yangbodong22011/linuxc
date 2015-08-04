/*************************************************************************
	> File Name: test_condition.c
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Aug 2015 12:53:19 PM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t   mutex;
pthread_cond_t    cond;


void *func3()
{
    while(1)
    {
        printf("thread3 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("thread3 抢到锁\n");
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
}
void *func1()
{
    while(1)
    {
        printf("thread----1 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("thread----1 抢到锁\n");
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
}

void *func2()
{
    while(1)
    {
        printf("thread2 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("thread2 抢到锁\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}
int main(int argc,char *argv[])
{
    pthread_t  tid1,tid2,tid3;

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_create(&tid1,NULL,func1,NULL);
    pthread_create(&tid2,NULL,func2,NULL);
    pthread_create(&tid3,NULL,func3,NULL);
    do{
        pthread_cond_signal(&cond);
    }while(1);
    pthread_cond_destroy(&cond);
    pthread_exit(0);
}
