/*************************************************************************
	> File Name: test_execl.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 08:55:42 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
    printf("entering main process----\n");
//    execl("/bin/ls","ls","-l","/home",NULL);
//    execlp("ls","ls","-l","/home",NULL);
    
    printf("exiting main process -----\n");

    return 0;
}
