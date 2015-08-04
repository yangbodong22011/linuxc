/*************************************************************************
	> File Name: test_pthread_exit.c
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Aug 2015 02:38:04 PM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
struct foo
{
    int a,b,c,d;
};

void printfoo(char *s,struct foo *fp)
{
    printf("%s\n",s);
    printf(" foo.a = %d\n",fp->a);
    printf(" foo.b = %d\n",fp->b);
    printf(" foo.c = %d\n",fp->c);
    printf(" foo.d = %d\n",fp->d);
}

void *func1()
{
    struct foo   foo = {1,2,3,4};
    printfoo("thread 1:",&foo);
    pthread_exit((void*)&foo);
}
int main(int argc,char *argv[])
{
    int          err;
    pthread_t    tid1,tid2;
    struct  foo  *fp;

    if((err = pthread_create(&tid1,NULL,func1,NULL)) != 0 )
        perror("pthread_exit error");
    if(( err = pthread_join(tid1,(void**)&fp)) != 0 )
        perror("pthread_join error");
    printfoo("parent:",fp);
    exit(0);
}
