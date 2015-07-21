/*************************************************************************
	> File Name: umask.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 06:55:30 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    umask(0);
    if(creat("file1",S_IRWXU|S_IRWXG|S_IRWXO) < 0)
    {
        perror("creat");
        exit(1);
    }
    umask(S_IRWXG);
    if(creat("file2",S_IRWXU|S_IRWXG|S_IRWXO) < 0)
    {
        perror("creat");
        exit(1);
    }
//file1的权限是777 file2的权限是707,因为第二个umask屏蔽了g权限
    

}
