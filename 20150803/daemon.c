/*************************************************************************
	> File Name: daemon.c
	> Author: 
	> Mail: 
	> Created Time: Mon 03 Aug 2015 12:51:27 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sys/stat.h>
#include<sys/param.h>

void create_daemon();

void create_daemon()
{
    int  pid;
    int  i;

    pid = fork();
    if(pid > 0)
        exit(0);
    setsid();
    pid = fork();
    if(pid > 0)
        exit(0);
    for(i = 0;i < NOFILE;close(i++))
        ;
    umask(0);
    chdir("/");
    signal(SIGCHLD,SIG_IGN);
}


int  main(int argc,char *argv[])
{
    printf("create a daemon\n");
    create_daemon();
   /* while(1)
    {
        printf("---------");
        sleep(1);
    }*/
    sleep(10);
   // printf("fathre process exit\n);
}
