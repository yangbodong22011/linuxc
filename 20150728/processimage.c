/*************************************************************************
	> File Name: processimage.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 12:16:00 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc,char *argv[],char ** environ)
{
    int i ;

    printf("i am a process image!\n");
    printf("my pid = %d,parentpid = %d\n",getpid(),getppid());
    printf("uid = %d,gid = %d\n",getuid(),getgid());

    for(i = 0;i < argc;i++)
    printf("argv[%d]:%s\n",i,argv[i]);
}
