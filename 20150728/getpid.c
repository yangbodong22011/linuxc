/*************************************************************************
	> File Name: getpid.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 03:14:02 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc ,char *argv[])
{
    int pid;

    if((pid = fork()) == -1)        //注意fork函数的返回值，子进程的返回值为0，父进程返回子进程的ID
    {
        perror("fork error");
        exit(1);
    }

    if(pid == 0)                    //针对子进程的情况 
    {
        printf("当前进程的ID是%d\n",getpid());  //当前ID是子进程的PID
    }
    getchar();
    exit(0);
}
