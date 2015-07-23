/*************************************************************************
	> File Name: my_chown.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 03:57:54 PM CST
 ************************************************************************/
 /*原来写的my_chown.c  由于chown函数必须传入参数uid和gid 这将限制它的功能，所以就想把它变变，现在直接传入用户名就
 OK啦，毕竟用户名还是好记一点。
 注意运行是要是root身份啊，毕竟chown,还有*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main(int argc ,char *argv[])
{
    struct stat buf;        //stat函数用来读出file文件的信息
    int fd,ret;             //ret chown函数的返回值
    char read_buf[10000];
    int i,k,w,j = 0,re = 0;
    char size[50];
    char uid1[10];
    char gid1[10];
    int uid2,gid2;          //将uid1[10] 和 gid1[10] 用atoi 函数转化成int型

    /*读出/etc/passwd文件的内容，目的是为了将输入的用户名转化成uid和gid*/
    if((fd = open("/etc/passwd",O_RDONLY)) == -1)
    {
        perror("open error");
        exit(1);
    }
    if(read(fd,read_buf,10000) < 0)
    {
        perror("read error");
        exit(1);
    }

    /*将第一个参数username 转化成与其对应的uid*/
       for(i = 0;i < 10000; i++)
    {
        if(read_buf[i] == argv[1][re] && re < strlen(argv[1]) )
             re++;
        else
            re = 0;
        if(re == strlen(argv[1]))
            break;
    }
    k = i;
    for(j = 0;i < k+20;i++,j++)
         size[j] = read_buf[i];
    for(j = 0,w = 0;j < 20; j++)
    {
        if('0' <= size[j] && size[j] <= '9')
            {
             uid1[w] = size[j];
             w++;
            if(w == 4)
            break;
        }
    }
    uid2 = atoi(uid1);           

   
  /*将第二个参数groupname 转化成与其对应的gid*/ 
    re = 0;
       for(i = 0;i < 5000; i++)
    {
        if(read_buf[i] == argv[2][re] && re <= strlen(argv[2]) )
             re++;
        else
            re = 0;
        if(re == strlen(argv[2]))
            break;
    }
    k = i;
    for(j = 0;i < k+50;i++,j++)
         size[j] = read_buf[i];
    for(j = 0,w = 0;j < 20; j++)
    {
        if('0' <= size[j] && size[j] <= '9')
        {
             gid1[w] = size[j];
             w++;
            if(w == 4)
            break;
        }
    }
    for(j ,w = 0;j < 20; j++)
    {
        if('0' <= size[j+1] && size[j+1] <= '9')
        {
             gid1[w] = size[j+1];
             w++;
            if(w == 4)
            break;
        }
    }
    gid2 = atoi(gid1);

    if((fd = open("file",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU)) == -1)
    {
        perror("open error");
        exit(1);
    }


    /*修改权限*/
    if((ret = chown("file",uid2,gid2)) == -1)
    {
        perror("chown");
        exit(1);
    }
    /*获得权限并且显示*/
    if((ret = stat("file",&buf)) == -1)
    {
        perror("stat");
        exit(1);
    }

    printf("\n\nfile的文件uid_t:gid_t:%d : %d\n\n",buf.st_uid,buf.st_gid);

}
