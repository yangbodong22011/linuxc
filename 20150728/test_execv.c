/*************************************************************************
	> File Name: test_execv.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 09:54:15 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

int main(void)
{
    int ret;

    printf("entering main process----\n");
    char *argv[] = {"ls","-l","/home",NULL};
 //   ret = execv("/bin/ls",argv);
    ret = execvp("ls",argv);
    if(ret == -1)
    {
        perror("execvp error");
    }
    printf("exiting main process-----\n");
    return 0;
    
}
