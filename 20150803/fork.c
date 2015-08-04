/*************************************************************************
	> File Name: fork.c
	> Author: 
	> Mail: 
	> Created Time: Mon 03 Aug 2015 03:44:30 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void create_child()
{
    int pid;
    pid = fork();
    if(pid > 0 )
        exit(0);
}


int main(int argc,char *argv[])
{
    printf("create a new process\n");
    create_child();
        printf("hello\n");
}
