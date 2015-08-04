/*************************************************************************
	> File Name: change_child.c
	> Author: 
	> Mail: 
	> Created Time: Mon 03 Aug 2015 04:13:20 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int globvar = 5;

int main(int argc,char *argv[])
{
    int pid;
    pid = vfork();
    switch(pid)
    {
        case 0:
            printf("child globvar:%d\n",globvar);
            sleep(2);
            break;
        default:
            globvar++;
            printf("father globvar:%d\n",globvar);
            exit(0);
    }
}
