/*************************************************************************
	> File Name: my_ping.c
	> Author: 
	> Mail: 
	> Created Time: Mon 07 Sep 2015 19:21:26 CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

//此程序的目的在于模拟ping的工作原理，

void handler_sigalarm(int signo)
{
    printf("send a icmp echo request packet\n");
    alarm(1);
}

int main(int argc,char *argv[])
{
    signal(SIGALRM,handler_sigalarm); //signal 函数在此处理SIGALRM，

    raise(SIGALRM);
    while(1);

    return 0;
}
