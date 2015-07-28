/*************************************************************************
	> File Name: test_signal.c
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Jul 2015 08:24:46 AM CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
int main(int argc ,char *argv[])
{
    signal(SIGINT,SIG_IGN);
    for(;;);
    return 0;
}
