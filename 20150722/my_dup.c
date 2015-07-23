/*************************************************************************
	> File Name: my_dup.c
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Jul 2015 09:03:38 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
    int oldfd;
    int newfd;

    oldfd = open("file1",O_RDWR|O_CREAT,0644);
    dup2(oldfd,newfd);
    write(newfd,"wwwww",4);
    close(oldfd);
}
