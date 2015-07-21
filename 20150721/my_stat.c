/*************************************************************************
	> File Name: my_stat.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 10:47:25 CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>


int main(int argc,char *argv[])
{
    int fd;
    int ret;
    struct stat buf;

    /*file1的建立*/
    if((fd = open("file1",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR)) == -1)
    {
        perror("open");
        exit(1);
    }
    /*判断参数的合理性*
    if(argc < 3)
    {
        perror(argc);
        exit(1);
    }*/

    if((ret = stat("file1",&buf)) == -1)
    {
        perror("stat");
        exit(1);
    }

    printf("%d\n",buf.st_uid);
    printf("%d\n",(int)buf.st_dev);
    

    



}
