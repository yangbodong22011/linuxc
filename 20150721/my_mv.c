/*************************************************************************
	> File Name: my_mv.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 07:08:04 PM CST
 ************************************************************************/

#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    int ret;

    if((ret = rename("file","file1")) < 0)
    {
        perror("rename error");
        exit(1);
    }

}
