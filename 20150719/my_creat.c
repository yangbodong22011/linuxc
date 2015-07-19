/*************************************************************************
	> File Name: my_creat.c
	> Author: 
	> Mail: 
	> Created Time: Sun 19 Jul 2015 18:54:25 CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    int fd;
    if((fd = open("/home/student/linuxc/20150719/creatfile",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR)) == -1)
      {
          printf("打开出错\n");
          perror("open");
       exit(1);
      }
    else
       printf("文件创建成功\n");
       close(fd);

       return 0;

}

