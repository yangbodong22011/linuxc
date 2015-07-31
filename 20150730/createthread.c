/*************************************************************************
	> File Name: createthread.c
	> Author: 
	> Mail: 
	> Created Time: Thu 30 Jul 2015 11:26:43 AM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

void creatfile()
{
    printf("nihao \n");

}


int main(int argc,char *argv[])
{
    pthread_t thid;

    printf("the pid is %d\n",getpid());
    printf("main thread ,ID is %u\n",pthread_self());            //打印主线程的ID
    if(pthread_create(&thid,NULL,(void *)creatfile,NULL) != 0)
    {
        printf("thread creation fail\n");
        exit(1);
    }
    printf("此进程的ID 是%d pid %d\n",thid,getpid());
    sleep(0.99999999999999999);                                  //17个9和1是相等的
    exit(0);
}
