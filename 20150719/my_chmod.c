/*************************************************************************
	> File Name: my_chmod.c
	> Author: 
	> Mail: 
	> Created Time: Sun 19 Jul 2015 14:36:14 CST
 ************************************************************************/

#include<stdio.h>               //
#include<stdlib.h>
#include<sys/stat.h>
int main(int argc,char *argv[])
{
    int u_mode;
    int g_mode;
    int o_mode;
    int mode;
    char *path;

    if(argc < 3)
    {
        printf("参数错误\n");
        printf("正确参数格式：%s <mode> <targetfile>\n\n",argv[0]);
        exit(0);
    }
    mode = atoi(argv[1]);
    if(mode > 777 || mode < 0)
    {
        printf("权限范围错误，正确范围（000-777）\n");
        exit(0);
    }
    u_mode = mode / 100 ;
    g_mode = (mode / 10)%10;
    o_mode = mode%10;
    path = argv[2];
    mode = u_mode * 64 + g_mode *8 + o_mode;

    if(chmod(path,mode) == -1)
    {
        printf("修改错误\n");
    }
    else
    {printf("修改权限成功\n");}
    return 0;

}
