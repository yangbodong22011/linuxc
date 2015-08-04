/*************************************************************************
	> File Name: test_mutex.c
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Aug 2015 10:27:59 AM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
pthread_mutex_t  number_mutex;                      //定义一把锁
int              globvar = 1;                       //定义一个全局变量globvar = 1

void *func1()
{
    pthread_mutex_lock(&number_mutex);              //加锁函数
    globvar++;
    printf("thread1---globvar:%d\n",globvar);
    pthread_mutex_unlock(&number_mutex);            //解锁函数 
    pthread_exit(0);
}
void *func2()
{
    pthread_mutex_lock(&number_mutex); 
    globvar++;
    printf("thread2---globvar:%d\n",globvar);
    pthread_mutex_unlock(&number_mutex);
    pthread_exit(0);
}
void *func3()
{
    pthread_mutex_lock(&number_mutex);
    globvar++;
    printf("thread3---globvar:%d\n",globvar);
    pthread_mutex_unlock(&number_mutex);
    pthread_exit(0);
}

int main(int argc,char *argv[])
{
    pthread_t  tid1,tid2,tid3;
    void *status1;
    void *status2;
    void *status3;

    pthread_mutex_init(&number_mutex,NULL);       //对锁进行初始化
    pthread_create(&tid1,NULL,func1,NULL);
    pthread_create(&tid2,NULL,func2,NULL);
    pthread_create(&tid3,NULL,func3,NULL);
    pthread_join(tid1,(void**)&status1);
    pthread_join(tid2,(void**)&status2);
    pthread_join(tid3,(void**)&status3);
    exit(0);

}
