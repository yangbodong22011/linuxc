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
    char * const envp[] = {"aa=11","bb=22","cc=33",NULL};

    printf("entering main process----\n");
 //   ret = execl("./test_execle_child","test_execle_child",NULL);
    ret = execle("./test_execle_child","test_execle_child",NULL,envp);
    if(ret == -1)
    {
        perror("execle error");
    }
    printf("exiting main process-----\n");
    return 0;
    
}
