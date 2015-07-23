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
    /*将我们当前工作目录转化成argv[1]*/
    if(chdir(argv[1]) < 0)        //argv[1] ： 我们想要切入的目录
    {
        perror("chdir error");
        exit(1);
    }
    
   /*为了验证正确性，我们在转化后的目录下建立名叫file2的文件*/
   fd =  open("file2",O_CREAT,S_IRUSR);

    /*获取当前目录，并输出*/
    getcwd(buf,100);
    printf("%s\n",buf);

    /*注意！ 程序结束后并不会直接转化到argv[1] ，但是在argv[1] 下会有file2存在，前提是执行此程序的用户
     对于在argv[1]目录下有 w 权限，至于为什么不能转换，现在无法解释，有关进程的东西下周学完再说*/

}
