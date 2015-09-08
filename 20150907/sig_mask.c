/*************************************************************************
	> File Name: sig_mask.c
	> Author: 
	> Mail: 
	> Created Time: Tue 08 Sep 2015 16:46:42 CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<stdlib.h>

void my_err(char *err_string,int line)
{
    fprintf(stderr,"line:%d\n",line);
    perror(err_string);
    exit(1);
}

void sighandel(int signo)
{
    printf("recv SIGINT\n");
}

int main(int argc,char *argv[])
{
    sigset_t  newmask,oldmask,pendmask;

    signal(SIGINT,sighandel);                     //设置SIGINT的捕捉函数

    sleep(10);                                    

    sigemptyset(&newmask);                        //对newmask信号集清空
    sigaddset(&newmask,SIGINT);                   //将信号SIGINT添加到newmask中

    sigprocmask(SIG_BLOCK,&newmask,&oldmask);     //屏蔽信号SIGINT,用的是newmask，原来的信号屏蔽码被返回到oldmask中

    sleep(10);

    sigpending(&pendmask);                        //获取未决信号队列到pendmask中

    switch(sigismember(&pendmask,SIGINT))         //检测信号SIGINT是否在PENFMASK队列中
    {
        case 0:
        {
            printf("SIGINT is not in queue\n");
            break;
        }
        case 1:
        {
            printf("SIGINT is in queue\n");
            break;
        }
        case -1:
        {
            my_err("sigismember",__LINE__);
            break;
        }
        default:
        break;
    }

    sigprocmask(SIG_SETMASK,&oldmask,NULL);       //取消屏蔽

    while(1);



}
