/*************************************************************************
	> File Name: mkdir.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 07:52:57 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<pwd.h>
#include<dirent.h>


int main(int argc,char *argv[])
{
    char buf[100];

    if(mkdir("test",S_IRWXU) < 0)
    {
        perror("mkdir error");
        exit(1);
    }
    printf("目录创建成功\n");

    /*
    if(rmdir("test") < 0)
    {
        perror("rmdir error");
        exit(1);
    }
    printf("目录删除成功\n");*/
    
    //获取当前目录
    if(getcwd(buf,100) == 0)
    {
        perror("getcwd error");
        exit(1);
    }
    puts(buf);
    printf("%d\n",PATH_MAX);







}
