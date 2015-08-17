/*************************************************************************
	> File Name: my_recv.c
	> Author: 
	> Mail: 
	> Created Time: Mon 10 Aug 2015 08:03:58 PM CST
 ************************************************************************/
#define MY_RECV_C
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}


