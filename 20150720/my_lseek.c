/*************************************************************************
	> File Name: my_lseek.c
	> Author: 
	> Mail: 
	> Created Time: Mon 20 Jul 2015 10:57:56 CST
 ************************************************************************/

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char *argv[])
{
    int fd,ret;
   // char write_buf[40] = "yangbodongshitiancai";
    char read_buf[40];
    char write_buf[40];
    int p;

    if((fd = open("file2",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR)) == -1)
    {
        perror("open");
        exit(1);
    }
    printf("请输入要写入的内容\n");
    scanf("%s",write_buf);
    if( write(fd,write_buf,strlen(write_buf)) != strlen(write_buf))
    {
        perror("write");
        exit(1);
    }
  //  printf("write的返回值ret:%d\n",ret);
    if((p =lseek(fd,3,SEEK_SET)) == -1)
    {
        perror("lseek");
        exit(1);
    }
    printf("lseek函数的返回值p:%d \n",p);
    if((ret =read(fd,read_buf,9) ) < 0)
      {
          perror("read");
          exit(1);
      }
    printf("文件描述符fd:%d \n",fd);
    printf("read函数的返回值ret:%d \n",ret);
     printf("%s \n",read_buf);
    for(ret=0;ret<=10;ret++)
        printf("%d\n",read_buf[ret]);
}
