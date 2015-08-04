/*************************************************************************
	> File Name: fork_and_fork.c
	> Author: 
	> Mail: 
	> Created Time: Mon 03 Aug 2015 08:40:04 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main(int argc,char *argv[])
{
    int pid1,pid2,pid3;
    printf("my pid is %d\n",getpid());
    pid1 = fork();
    if(pid1 == 0);
    {
        printf("i am child process my pid is %d\n",getpid());
    }
    pid2 = fork();
    if(pid2 == 0)
    {
        printf("i am child process my pid is %d\n",getpid());
    }
    pid3 = fork();
    if(pid3 == 0)
    {
        printf("i am child process my pid is %d\n",getpid());
    }
  //  printf("my pid is %d\n",getpid());
}
