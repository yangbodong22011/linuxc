/*************************************************************************
	> File Name: my_sigaction.c
	> Author: 
	> Mail: 
	> Created Time: Mon 07 Sep 2015 16:04:43 CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int temp = 0;

void handler_sigint(int signo)
{
    printf("recv SIGINT\n");
    sleep(5);
    temp += 1;
    printf("the value of temp is : %d\n",temp);
    printf("in handler_sigint,after sleep\n");
}

int main(int argc,char *argv[])
{
    struct sigaction act;

    act.sa_handler = handler_sigint;
    //act.sa_flags   = SA_NOMASK;      //注释此行后，由于SIGINT是不可靠信号，不可靠信号不支持排队，所以会使信号丢失

    sigaction(SIGINT,&act,NULL);

    while(1)
    ;

    return 0;
    
}
//Q1:为什么显示的时候是一并显示的呢？
