/*************************************************************************
	> File Name: wrong_return.c
	> Author: 
	> Mail: 
	> Created Time: Mon 07 Sep 2015 16:35:47 CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<setjmp.h>

jmp_buf   env;

void handler_sigrtmin15(int signo)
{
    printf("recv SIGRTMIN+15\n");
    longjmp(env,1);   //返回到env处，注意第二个参数的值
}

void handler_sigrtmax15(int signo)
{
    printf("recv SIGRTMAX-15\n");
    longjmp(env,2);   //返回到env处，注意第二个参数的值
}

int main(int agrc ,char *argv[])
{
    switch(setjmp(env))                 //setjmp函数直接调用返回0
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            printf("return from SIGRTMIN+15\n");
            break;
        }
        case 2:
        {
            printf("return from SIGRTMAX-15\n");
            break;
        }
        default:
            break;
    }

    signal(SIGRTMIN+15,handler_sigrtmin15);
    signal(SIGRTMAX-15,handler_sigrtmax15);

    while(1);

    return 0;
}


