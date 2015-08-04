/*************************************************************************
	> File Name: test_con.c
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Aug 2015 08:30:22 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t  mutex;
pthread_cond_t   cond;
int              var = 0;
void *func2()
{
    while(var < 10)
    {
            printf("thread2 is runninig\n");
            pthread_mutex_lock(&mutex);
            if(var%2 !=  0)
                pthread_cond_wait(&cond,&mutex);//如果是奇数就让它阻塞
            printf("pthread 2 var :%d\n",var); //如果是偶数，就让它输出
            pthread_mutex_unlock(&mutex);
            sleep(1);
    }
}

void *func1()
{
    pthread_cleanup_push(pthread_mutex_unlock,&mutex);
    for(var = 0;var <= 10;var++)
    {
        
            printf("thread1 is running\n");
            pthread_mutex_lock(&mutex);
            if(var%2 == 0)  //如果是偶数，就发出信号，唤醒上面的wait,但是wait能不能
                pthread_cond_signal(&cond); //收到信号与它是没有关系的,它就会继续往下执行
            else
                printf("pthread 1 var :%d\n",var);
            pthread_mutex_unlock(&mutex);
            sleep(1);
        
    }
    pthread_cleanup_pop(0);
}

int main(int argc,char *argv[])
{
    pthread_t   tid1,tid2;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_create(&tid1,NULL,func1,NULL);
    pthread_create(&tid2,NULL,func2,NULL);
    
    sleep(10); 
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
