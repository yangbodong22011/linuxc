/*************************************************************************
	> File Name: wait_cycle.c
	> Author: 
	> Mail: 
	> Created Time: Tue 08 Sep 2015 21:14:14 CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>

#define UN   0
#define HA   1

int  flag;

void handler_sigint(int signo)
{
    flag = HA;
}

int main(int argc,char *argv[])
{
    signal(SIGINT,handler_sigint);

    while(1)
    {
        if(flag == HA)
        {
            printf("event happened\n");
            break;
        }
    }

    return 0;
}
