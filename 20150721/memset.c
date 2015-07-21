/*************************************************************************
	> File Name: memset.c
	> Author: 
	> Mail: 
	> Created Time: Tue 21 Jul 2015 09:30:00 CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main(int argc,char* argv[])
{
    char buf[20];
    memset(buf,0,20);
    memcpy(buf,"hel",3);
    printf("%s\n",buf);
    printf("%p\n",buf);
}
