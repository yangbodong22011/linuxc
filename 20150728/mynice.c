/*************************************************************************
	> File Name: mynice.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 04:25:00 PM CST
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/resource.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc ,char *argv[])
{
    int    pid;
    int    stat_val = 0;
    int    oldpri,newpri;

    printf("nice study\n");
    pid = fork();
    switch(pid)
    {
        case 0:
        printf("child is running,curpid is %d,parentpid is %d\n",pid,getpid());

        oldpri = getpriority(PRIO_USER,0);
        printf("old priority is %d\n",oldpri);
        newpri = nice(-11);
        printf("new priority is %d\n",newpri);
        exit(0);
        case -1:
        printf("creat process failed\n");
        break;
        default:
        printf("parent is running,childpid is %d, parentpid is %d \n",pid,getpid());
        break;
    }
    wait(&stat_val);
    exit(0);
}
