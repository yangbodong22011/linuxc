/*************************************************************************
	> File Name: tsd.c
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Aug 2015 08:58:27 AM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

pthread_key_t  key;                          //申请一个key
void *func2()
{
    int tsd = 2;
    pthread_setspecific(key,(void*)&tsd);       //将key与pthread2的变量tsd相关联
    printf("i am thread2 :%u,my tsd:%d\n",pthread_self(),tsd);//输出pthread2的变量
    pthread_exit(0);
}

void *func1()
{
    int tsd = 0;
    int *status2;
    pthread_t tid2;

    pthread_setspecific(key,(void*)&tsd);       //将key与pthread1的变量tsd相关联
    printf("i am thread1 :%u,my tsd:%d\n",pthread_self(),tsd);//输出pthread1的变量
    pthread_create(&tid2,NULL,func2,NULL);
    pthread_join(tid2,(void**)&status2);
    pthread_exit(0);

}
int main(int argc,char *argv[])
{
    pthread_t  tid1,tid2;
    int *status;
    pthread_key_create(&key,NULL);              //创建一个key

    pthread_create(&tid1,NULL,func1,NULL);
    pthread_join(tid1,(void**)&status);
    pthread_key_delete(key);                    //删除key
    
    exit(0);
}
