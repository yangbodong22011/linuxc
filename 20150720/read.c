/*************************************************************************
	> File Name: read.c
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Jul 2015 08:12:11 CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

int main(int argc,char *argv[])
{
    int i;
    int fd;
    int ret;
    int count = 20;
    char read_buf[40];
    char write_buf[20] = "hdhdhdhdhdhdhdhd";

    if((fd = open("file.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR)) == -1)
    {
        printf("打开失败\n");
        perror("open");
    }
    printf("%d \n",fd);
    if( write(fd,write_buf,strlen(write_buf) )!= strlen(write_buf))
      {
          perror("write");
      }
    printf("%d \n",fd);
    i =  lseek(fd,3,SEEK_END);
    if((ret = read(fd,read_buf,count)) < 0)    //read的返回值表示实际读取到的字节数  0：count为零，或者已经读到末尾。 
    {
        printf("读取失败\n");
        perror("read");
    }
    else
    printf("lseek函数返回值%d\n",i);
    printf("%d \n",ret);
    printf("%d \n",fd);
    printf("%s \n",read_buf);
}

