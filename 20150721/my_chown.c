/*************************************************************************
	> File Name: my_chown.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 03:57:54 PM CST
 ************************************************************************/
/*此程序需要用root用户来执行*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>


int main(int argc ,char *argv[])
{
    int ret;
    struct stat buf;
    int uid,gid;
    
    uid = atoi(argv[1]);
    gid = atoi(argv[2]);
    //修改权限
    if((ret = chown("file",uid,gid)) == -1)
    {
        perror("chown");
        exit(1);
    }
    //获得权限并且显示
    if((ret = stat("file",&buf)) == -1)
    {
        perror("stat");
        exit(1);
    }

    printf("file的文件uid_t:gid_t:%d : %d\n",buf.st_uid,buf.st_gid);

}
