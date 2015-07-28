/*************************************************************************
	> File Name: wait.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 02:03:28 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    int     pid;
    char    *msg;
    int     k;
    int     exit_code;

    printf("study how to get exit code\n");
    pid = fork();
    printf("%d--------\n\n",pid);
    switch(pid)
    {
        case 0:
        printf("curpid = %d,parentpid = %d\n",pid,getppid());
        msg = "children process is running";
        k = 5;
        exit_code = 55;
        break;
        case -1:
        perror("process creat failed\n");
        exit(-1);
        default:
        printf("childpid is %d ,parentpid is %d\n",pid,getpid());
        exit_code = 0;
        break;
    }

    //父子进程都会执行下面的代码，子进程的pid为0；父进程的pid为子进程的ID
    printf("------------------------%d\n",pid);
    if(pid != 0)
    {
        int   stat_val;                         // 值为0
        int   child_pid;

        child_pid = wait(&stat_val);     //wait函数的返回值是终止运行的子进程的pid, 遇到wait函数之后开始执行子进程.
   //     printf("%d\n",child_pid);
  //      printf("stat_val:%d\n",stat_val);

        printf("child process has exited,pid = %d\n",child_pid);
  //      printf("WIFEXITED(stat_val) : %d\n",WIFEXITED(stat_val));
        if(WIFEXITED(stat_val))
        printf("child exited with code %d\n",WEXITSTATUS(stat_val));
        else
        printf("child exited abnormally\n");
    }
    //让子进程暂停5秒
    else 
    {
        while(k-- > 0)
        {
            puts(msg);
            sleep(3);
        }
    }
    exit(exit_code);
}
