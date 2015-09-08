/*************************************************************************
	> File Name: test_setitimer.c
	> Author: 
	> Mail: 
	> Created Time: Mon 07 Sep 2015 19:40:08 CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<sys/time.h>
#include<unistd.h>

void handler_sigtime(int signo)
{
    switch(signo)
    {
        case SIGALRM:
        {
            printf("recv SIGALRM\n");
            break;
        }
        case SIGPROF:
        {
            printf("recv SIGPROF\n");
            break;
        }
        default:
            break;
    }
}

int main(int argc,char *argv[])
{
    struct itimerval value;
    signal(SIGALRM,handler_sigtime);  
    signal(SIGPROF,handler_sigtime);

    value.it_value.tv_sec = 1;                 //注意需要给两个都赋值，秒和毫秒。
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 5;
    value.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL,&value,NULL);        //此函数相当于alarm，也用来设置定时器，且根据which不同的参数会有不同的信号返回       
    setitimer(ITIMER_PROF,&value,NULL);

    while(1);

    return 0;
}
