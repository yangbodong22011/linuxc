#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

jmp_buf jmpbuffer;
int fun1(void);
void fun2(void);
int fun1(void)
{
    char *s = "hello";
    char *s1 = "Hello";
    printf("1111111111111111\n");
    if(strcmp(s,s1)!=0)
//    longjmp(jmpbuffer,1);
    return 0;
    printf("222222222222222\n");

}

void fun2(void)
{
    char *s = "world";
    if(strcmp(s,"World")!=0)
    longjmp(jmpbuffer,1);          //因为setjmp会在jmpbuffer中保存栈的状态，调用longjump之后返回的地方其实是原来栈的状态，它的第二个参数就是setjmp的返回值
}


int main(void)
{
    int i = 0;
    int j = 0;
    i = setjmp(jmpbuffer);         //在此调用setjmp函数
    if(i==0)
    {
        printf("first run\n");
        fun1();
        fun2();
    }
    else
    {
        switch(i)
        {
            case 1:
                printf("In fun1 \n");
                break;
            case 2:
                printf("In fun2\n");
                break;
            default:
                printf("unkown error\n");
                break;
        }
        exit(1);
    }
    return 0;
} 

