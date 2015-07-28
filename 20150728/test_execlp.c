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
    execlp("ls","/home/kiosk/linuxc/",NULL);
    printf("exiting main process -----\n");

    return 0;
}
