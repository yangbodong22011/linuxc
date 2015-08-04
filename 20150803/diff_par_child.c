/*************************************************************************
	> File Name: diff_par_child.c
	> Author: 
	> Mail: 
	> Created Time: Mon 03 Aug 2015 10:42:34 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int globvar = 6;
int main(int argc,char *argv[])
{
    int pid;
    int var = 5;
    printf("now var is %d ,globvar is %d\n",var,globvar);
    pid = fork();
//    pid = vfork();
    switch(pid)
    {
        case 0:
            var++;
            globvar++;
            printf("child process var is %d, globvar is %d\n",var,globvar);
            exit(0);
        default:
            sleep(5);
            var++;
            globvar++;
            printf("father process var is %d, globvar is %d\n",var,globvar);
            exit(0);
            
    }
 exit(0);

}
