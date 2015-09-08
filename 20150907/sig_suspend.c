/*************************************************************************
	> File Name: sig_suspend.c
	> Author: 
	> Mail: 
	> Created Time: Tue 08 Sep 2015 17:13:23 CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void handler_sigint(int signo)
{
    printf("recv SIGINT\n");
}

int main(int argc,char *argv[])
{
    sigset_t   newmask,oldmask,zeromask;

    signal(SIGINT,handler_sigint);

    sigemptyset(&newmask);
    sigemptyset(&zeromask);
    sigaddset(&newmask,SIGINT);

    sigprocmask(SIG_BLOCK,&newmask,&oldmask);

    sigsuspend(&zeromask);

    sigprocmask(SIG_SETMASK,&oldmask,NULL);
    while(1);

    return 0;
    
}
