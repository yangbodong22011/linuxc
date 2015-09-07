/*************************************************************************
	> File Name: my_signal.c
	> Author: 
	> Mail: 
	> Created Time: Mon 07 Sep 2015 15:17:31 CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>

void hander_sigint(int signo)
{
    printf("recv SIGINT\n");
}


int main(int argc, char *argv[])
{
    signal(SIGINT,hander_sigint);   //signal函数是一个异步模型，当有信号来时，signal才会检测到，此时忽略用户按下的ctrl+c组合

    while(1)                        
        ;
    return 0;
}
