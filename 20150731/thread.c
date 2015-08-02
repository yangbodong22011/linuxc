/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: Fri 31 Jul 2015 09:21:11 AM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
void *child(void *argv)
{
    int pid;

    printf("这是线程在运行\n");
    if((pid = fork()) < 0)
        perror("");
    printf("%d-----\n",pid);
    switch(pid)
    {
        case 0:
            if(execl("/bin/ls","ls",argv,"/home",NULL)== -1)
            {
                perror("execl error");
            }
            exit(0);

        default:
            printf("father\n");
  //           sleep(3);
        /*sleep这条语句的执行有什么影响？*/
    }
   pthread_exit(0);

}

int main(int argc,char *argv[])          
{
    pthread_t thid;
    pthread_attr_t attr;
    int detachstate;
    int schedpolicy;
    int inheritsched;
    int scope;
    size_t guardsize;
    size_t stacksize;
    int *status;

    pthread_attr_init(&attr);
    /*在创建线程时，pthread_create函数的第二个参数为一个结构体，我们可以从结构体中获得线程的相关属性，但需要提前对这个结构体进行初始化，调用pthread_attr_init函数*/
 //   pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    /*我们可以通过设置线程的相关属性以达到我们想要的结果，上面这条语句就把线程的分离状态设置为可以与其他线程同步*/
    if(pthread_create(&thid,&attr,(void*)child,(void *)argv[1]) != 0) 
    {
        printf ("new thread create failed\n") ;
    }
    /*创建一个线程，我们创建的线程再去执行child函数中的内容，child函数的参数为argv[1]*/
    pthread_join(thid, (void**)(&status));
    /*将父进程挂起，保证子线程可以执行完成*/
 //   exit(0);
    /*若我们现在调用exit函数，下面的代码一定不会执行，但重点是我在线程中又fork了进程，那要是main函数先于子进程退出，子进程中的代码会不会执行？*/
    printf("father process----------\n");
    pthread_attr_getdetachstate(&attr,&detachstate);
    pthread_attr_getschedpolicy(&attr,&schedpolicy);
    pthread_attr_getinheritsched(&attr,&inheritsched);
    pthread_attr_getscope(&attr,&scope);
    pthread_attr_getguardsize(&attr,&guardsize);
    pthread_attr_getstacksize(&attr,&stacksize);
    printf("分离状态:%d\n 调度策略:%d\n 继承性:%d\n 作用域:%d\n 警戒堆栈大小:%d\n 堆栈大小:%d\n",detachstate,schedpolicy,inheritsched,scope,guardsize,stacksize);
 //   pthread_join(thid, (void**)(&status));
    /*两段pthread_join函数位置不同，有什么影响吗？*/

    

}
