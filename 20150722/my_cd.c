/*************************************************************************
	> File Name: my_cd.c
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Jul 2015 07:44:39 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
int main(int argc,char *argv[])
{
    char buf[100];
    int fd;
    //检查参数的合理性
    if(argc != 2)
    {
        perror("参数错误：");
        exit(1);
    }
    if(chdir(argv[1]) < 0)
    {
        perror("chdir error");
        exit(1);
    }

   fd =  open("file2",O_CREAT,S_IRUSR);
    printf("%d\n",fd);
    getcwd(buf,100);
    printf("%s\n",buf);
}
