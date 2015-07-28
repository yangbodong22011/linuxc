/*************************************************************************
	> File Name: hello.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 10:09:07 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

extern char ** environ;

int main(int argc,char *argv[])
{
    int i ;

    printf("\n\n现在已经进入main函数中的子程序---- pid=%d\n",getpid());
    sleep(2);
    for(i = 0;environ[i] != NULL;i++)
    {
        printf("%s\n",environ[i]);
    }
    return 0;

}
