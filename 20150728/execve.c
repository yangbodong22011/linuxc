/*************************************************************************
	> File Name: execve.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 12:20:48 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>

int main(int argc,char *argv[],char **environ)
{
    int pid;
    int stat_val;

    printf("Exec example!\n");
    pid = fork();
    switch(pid)
    {
        case -1:
        perror("process cerat faild\n");
        exit(1);
        case 0:
        printf("child process is running\n");
        printf("my pid = %d,parentpid = %d\n",getpid(),getppid());
        printf("uid = %d,gid = %d\n",getuid(),getgid());
        execve("processimage",argv,environ);
        printf("程序不会执行到这儿的\n");
        exit(0);
        default:
        printf("parent process is running\n");
        break;
    }
    wait(&stat_val);
    exit(0) ;
}

