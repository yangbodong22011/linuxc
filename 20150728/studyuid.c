/*************************************************************************
	> File Name: studyuid.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 03:45:40 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
extern int errno;

int main(int argc ,char *argv[])
{
    int fd;

    printf("学习理解内核对文件存取的许可权\n");
    printf("process uid = %d,euid = %d\n",getuid(),geteuid());
    
    if((fd = open("file",O_RDWR)) == -1)
    {
        printf("open fail,errno is %d:%s \n",errno,strerror(errno));
        exit(1);
    }
    else
    {
        printf("open successfully\n");
    }
    close(fd);
    exit(0);
}
