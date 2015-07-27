/*************************************************************************
	> File Name: env.c
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Jul 2015 09:11:53 PM CST
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>

extern char **environ;

int main(int argc,char *argv[],char *const envp[])
{
    int i;
    for(i = 0;environ[i] != NULL;i++)
    {
        printf("%s\n",environ[i]);
    }


   /* printf("Argument:\n"); 
    for(i = 0;i < argc;i++)
    {
        printf("argv[%d] is %s\n",i,argv[i]);
    }

    printf("Environment:\n");
    for(i = 0;environ[i] != NULL;i++)
    {
        printf("%s\n",environ[i]);
        printf("environ[i] = %d\n",environ[i]);
    }
        printf("%d\n",environ[i]);                      //为0时结束

    return 0;*/
}
