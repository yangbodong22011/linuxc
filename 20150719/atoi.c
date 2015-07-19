/*************************************************************************
	> File Name: atoi.c
	> Author: 
	> Mail: 
	> Created Time: Sun 19 Jul 2015 14:46:35 CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    char *head = "123.456";
    float p1 ;
    int p2;

    p1 = atoi(head);
    p2 = atoi(head);
    printf("%d   %f\n",p2,p1);

    return 0;
}
