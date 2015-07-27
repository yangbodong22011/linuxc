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

    printf("\t\t进程的创建学习\n");
    pid  = fork();
 //   printf("%d\n",pid);
    switch(pid)
    {
        printf("PID:%d\n",pid);
        case 0:
        while(1)
        {
            printf("A background process,PID:%d,ParentID:%d\n",getpid(),getppid());
            sleep(3);
        }
        break;
        case -1:
        perror("Process creation failed\n");
        exit(-1);
        default:
        printf("I am parent process ,my pid is %d\n",getpid());
        exit(0);
    }
    return 0;
}
