/*************************************************************************
	> File Name: my_fcntl.c
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Jul 2015 16:19:45 CST
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    int fd;
    int set1=2,set2=2;
    int mode;

    if((fd = open("file3",O_RDONLY|O_CREAT|O_TRUNC,S_IRUSR)) == -1)
    {
        perror("open");
        exit(1);
    }

    if((set1 = fcntl(fd,F_SETFL,O_NONBLOCK)) < 0)
    {
        perror("fcntl");
        exit(1);
    }
    if((set2 = fcntl(fd,F_GETFL,0)) < 0 )
    {
        perror("fcntl");
        exit(1);
    }
    printf("%d %d\n",set1,set2);
    mode = set2 & O_ACCMODE; 
    if(mode == 0)
    printf("file3的权限：read only");
    if(mode == 1)
    printf("file3的权限：write only");
    if(mode == O_RDWR)
    printf("file3的权限：read and only");

    if(set2 & O_APPEND)
    printf(",append\n");
    if(set2 & O_NONBLOCK)
    printf(",nonblock");
    if(set2 & O_SYNC)
    printf(",sync");
    printf("\n");
    return 0;


    
    
}
