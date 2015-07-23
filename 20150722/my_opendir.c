/*************************************************************************
	> File Name: my_opendir.c
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Jul 2015 08:33:58 AM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    DIR *dir;               //为opendir函数的返回值，是DIR * 形态的目录流，类似与文件描述符
    struct dirent *ptr;     //此结构体中包含目录的节点编号(long d_ino),目录文件开头到目录进入点的位移(off_t d_off)
                            //目录名称的长度(d_reclen),以NULL结尾的文件名(d_name)

    if((dir = opendir("test")) == NULL)  // 打开目录
    {
        perror("opendir error");
        exit(1);
    }
    while((ptr = readdir(dir)) != NULL)  //将目录信息读入结构体ptr中
    {
        printf("%s\n",ptr->d_name);      //输出目录下所有文件名以NULL结尾的文件
        
    }
    closedir(dir);

}
