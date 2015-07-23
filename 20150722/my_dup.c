/*************************************************************************
	> File Name: my_dup.c
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Jul 2015 09:03:38 AM CST
 ************************************************************************/
/*此程序可以解释shell命令中重定向“>”，是由dup函数对于标准输入输出的调用实现的*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
    int oldfd;
    int newfd;

    oldfd = open("file1",O_RDWR|O_CREAT,0644);
    dup2(oldfd,newfd);                  //用newfd 来代替 oldfd
    write(newfd,"wwwww",4);             //现在会将4个w写入file1 中 
    close(oldfd);

    /*shell命令的实现：
     [kiosk@yangbodong ~]$ echo "wwww" > file1
    此条命令就可以实现上面dup函数实现的功能，原因是 echo 是对标准输入0的操作，现在通过 ">" 对dup函数的
    调用，用file1的fd将标准输入的fd所替换，所以file1中就会有4个"w"*/
     
}
