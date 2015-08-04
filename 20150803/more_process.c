/*************************************************************************
	> File Name: more_process.c
	> Author: 
	> Mail: 
	> Created Time: Mon 03 Aug 2015 03:32:50 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    int pid1,pid2,pid3;
    pid1 = fork();
    switch(pid1)
    {
        case 0:
        {
            printf("child process 1,pid:%d\n",getpid());
            pid2 = fork();
            switch(pid2)
            {
                case 0:
                    printf("child process2,pid:%d\n",getpid());
                    break;
                default:
                    printf("father process2,pid:%d\n",getpid());
                    exit(0);
            }
            break;
        }
        default:
            printf("father process1,pid:%d\n",getpid());
    }
}
