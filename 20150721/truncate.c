/*************************************************************************
	> File Name: truncate.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 05:26:28 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<stdlib.h>
#include<fcntl.h>
/*从此程序中得到一个经验，vim一个文件之后保存会在其后默认加\n*/
int main(int argc,char *argv[])
{
    int ret;
    int fd;
    char read_buf[12];

    if((ret = truncate("file",20)) == -1)
    {
        perror("truncate");
        exit(1);
    }
    fd = open("file",O_RDWR);
    lseek(fd,0,SEEK_SET);
    if(read(fd,read_buf,12) != 12)
    {
        perror("read");
        exit(1);
    }
    printf("%s\n",read_buf);
}

