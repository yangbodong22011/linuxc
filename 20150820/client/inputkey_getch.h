/*************************************************************************
	> File Name: 3.c
	> Author: 
	> Mail: 
	> Created Time: 2015年06月30日 星期二 21时44分27秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<assert.h>
#include<string.h>
int getch1(void)
{
    struct termios tm,tm_old;
    int fd = STDIN_FILENO, c,d;
    if(tcgetattr(fd, &tm) < 0)
    {
        return -1;
    }
    tm_old = tm;
    cfmakeraw(&tm);
    if(tcsetattr(fd, TCSANOW, &tm) < 0)
    {
        return -1;
    }
    c = fgetc(stdin);
    if(tcsetattr(fd, TCSANOW, &tm_old) < 0)
    {
        return -1;
    }
    return c;
}


int inputkey(char k[])
{
    int i;
    for(i = 0; i < 10; i++)
    {
        __fpurge(stdin);
        k[i] = getch1();
        if((k[i] == 127) && (i == 0))
        {
            i--;
            continue ;
        }
        if(k[i] == 13)
        {
            k[i]=0;
            break ;
        }
        else if((k[i] == 127) && (i > 0))
        {
            printf("\b \b");
            i-=2;
        }
        else if(k[i] == 3)
        {
            printf("\n");
            exit(0);
        }
        else
        {
            printf("*");
        }
    } 
}
