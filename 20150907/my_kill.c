/*************************************************************************
	> File Name: my_kill.c
	> Author: 
	> Mail: 
	> Created Time: Mon 07 Sep 2015 18:51:08 CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>

int main(int argc,char *argv[])
{
    int   i,j;
    int   signum = SIGTERM;
    pid_t pid;

    for(i = 1;i < argc;i++)
    {
        if(!strcmp(argv[i],"-s"))
        {
            signum = atoi(argv[i+1]);       //信号量就是-s之后的数值
            break;
        }
    }
    if(argc == 2)                                                         
    {
        pid = atoi(argv[1]);
    }
    else
    {
        for(j = 1;j < argc;j++)
        {
            if((j != i) && (j != i+1))
            {
                pid = atoi(argv[j]);
                break;
            }
        }
    }


    if(kill(pid,signum) < 0)        //kill函数pid分为四类，>0 表示某个进程的pid，=0 表示进程将信号发送给进程组中的所有进程
    {                               //=-1 表示将信号广播给除了init进程和自己的所有进程 <-1 表示发送给属于进程组-pid的的进程
        perror("kill");
        exit(1);
    }

    return 0;


}
