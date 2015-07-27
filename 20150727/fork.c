/*************************************************************************
	> File Name: fork.c
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Jul 2015 10:59:22 AM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    pid_t pid;
    printf("创建进程学习\n");
    pid  = fork();
    printf("%d\n",pid);
    switch(pid)
    {
        case 0:
        printf("子进程在运行，Pid:%d,父进程Pid:%d\n",pid,getppid());
        break;
        case -1:
        perror("创建失败\n");
        break;
        default:
        printf("父进程在运行，子进程Pid:%d,父进程Pid:%d\n",pid,getppid());
        break;
    }
    exit (0);
}
