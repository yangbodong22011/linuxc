/*************************************************************************
	> File Name: myshell.c
	> Author: 
	> Mail: 
	> Created Time: Wed 29 Jul 2015 08:25:26 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

void my_print();
void get_input(char *buf);
void explain_input(char *buf,int *argcount,char (*)[256]);

void explain_input(char *buf,int *argcount,char (*arglist)[256])  //此函数传进来的变量如果改变就需要保存它的值。
{
    char *p = buf;
    char *q = buf;
    int number = 0;
    while(1)
    {
        if(p[0] == '\n')
        break;
        if(p[0] == ' ')
        p++;
        else
        {
            q = p;
            number = 0;
            while((q[0] != ' ') && (q[0] != '\n'))
            {
                number++;
                q++;
            }
            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
}
void get_input(char *buf)
{
    int len = 0;
    int ch;

    ch = getchar();
    while(len < 256 && ch != '\n')
    {
        buf[len++] = ch;
        ch = getchar();
    }
    buf[len] = '\n';
    len++;
    buf[len] = '\0';
}

void my_print()
{
    printf("shell$$ ");
}

int main(int argc,char *argv[])
{
    int i;
    char *buf = NULL;
    int argcount = 0;
    char arglist[100][256];
    char *arg[20];
    int pid;
    int stat_val;

    buf = (char *)malloc(256);
    while(1)
    {
        memset(buf,0,256);
        my_print();
  //      fflush(NULL);
        //处理输入参数的部分
        get_input(buf);
        for(i = 0;i < 100;i++)
        {
            arglist[i][0] = '\0';
        }
        argcount = 0;
        explain_input(buf,&argcount,arglist);
        for(i = 0;i < argcount;i++)
            arg[i] = &arglist[i][0];
        for(i = 0;i < argcount;i++)
            printf("%s ",arg[i]);
        if((pid = fork()) < 0)
        {
            perror("fork error");
            return 0;
        }
        switch(pid)
        {
            case 0:
                execvp(arg[0],arg);
                exit(0);
                break;
            default:
                if(waitpid(0,&stat_val,0) == -1)
                    perror("error");
        }
    }
    exit(0);
}

