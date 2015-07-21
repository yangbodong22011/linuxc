/*************************************************************************
	> File Name: ulink.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 07:25:54 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
/*有的安装包需要在安装后自己删除某些文件，这样就可以实现此功能*/
int main(int argc,char *argv[])
{
    int fd;
    int ret;
    char write_buf[20] = "aaaaaaaaaaaaaaaaaaa";
    char read_buf[20]; 
    
    if((fd = open("file1",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU)) == -1)
    {
        perror("open error");
        exit(1);
    }
    if((ret = unlink("file2")) < 0)
    {
        perror("unlink error");
        exit(1);
    }
    printf("删除成功\n");
    if(write(fd,write_buf,10) < 0)
    {
        perror("write error");
        exit(1);
    }
    if(lseek(fd,0,SEEK_SET)  < 0)
    {
        perror("lseek error");
        exit(1);
    }
    if(read(fd,read_buf,10) < 0)
    {
        perror("read error");
        exit(1);
    }
    printf("%s\n",read_buf);
}
