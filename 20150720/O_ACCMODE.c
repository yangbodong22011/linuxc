/*************************************************************************
	> File Name: O_ACCMODE.c
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Jul 2015 16:04:24 CST
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>

int main(int argc,char* argv[])
{
    printf("%d %d %d %d %d %d %d\n",O_ACCMODE,O_RDONLY,O_WRONLY,O_RDWR,O_APPEND,O_NONBLOCK,O_SYNC);
}
