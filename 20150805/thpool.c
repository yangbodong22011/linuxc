/*************************************************************************
	> File Name: thpool.c
	> Author: 
	> Mail: 
	> Created Time: Wed 05 Aug 2015 02:14:10 PM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

typedef void*(*FUNC)(void* arg);                //指向函数的指针，用来指向我的线程函数。
typedef struct _thpool_jobqueue                 //任务队列
{
    FUNC                     routine;           //定义指向线程函数的指针routine
    void*                    arg;               //传向线程函数的参数
    struct _thpool_jobqueue  *next;
}thpool_jobqueue;
typedef struct _thpool_t                        //线程池
{
    int               max_thr_num;              //线程池的尺寸
    int               flag;                     //是否销毁的标志
    pthread_t         *thr_id;                  //线程ID指针
    pthread_cond_t    jobqueue_cond;            //条件变量
    pthread_mutex_t   jobqueue_mutex;           //互斥锁
    thpool_jobqueue   *jobqueue_head;           //指向任务队列的指针
}thpool_t;
static thpool_t * thpool = NULL;                //全局变量，指向线程池的指针

/*建议：为了结构更清晰，请从main函数开始，然后线程创建函数--->任务添加函数--->线程函数*/


void *thread_routine()                          //线程函数
{
    thpool_jobqueue *work;                        
    while(1)
    {
        pthread_mutex_lock(&thpool->jobqueue_mutex);              
        while((thpool->jobqueue_head == NULL) &&( thpool->flag == 0)) //如果此时没有任务，并且不打算销毁线程池
        {
            pthread_cond_wait(&thpool->jobqueue_cond,&thpool->jobqueue_mutex);//       就让抢到锁的在此等待，其他线程在锁的外边阻塞
        }
        if(thpool->flag != 0)                                         //每次可以执行的时候都判断下线程池是否要销毁
        {
            pthread_mutex_unlock(&thpool->jobqueue_mutex);
            pthread_exit(0);
        }
        work = thpool->jobqueue_head;                                 //若不销毁，则将任务添加到任务队列
        thpool->jobqueue_head = thpool->jobqueue_head->next;          //让任务指针指向下一个
        pthread_mutex_unlock(&thpool->jobqueue_mutex);
        work->routine(work->arg);                                     //work->routine = routine = func1
        free(work);
    }
}

void thpool_add_task(void*(*routine)(void *),void* arg)        //任务添加函数
{
    thpool_jobqueue  *work,*member;
    
    work = (thpool_jobqueue*)malloc(sizeof(thpool_jobqueue));  //准备任务，将添加到任务队列
    work->routine = routine;
    work->arg = arg;
    work->next = NULL;
    pthread_mutex_lock(&thpool->jobqueue_mutex);               //对任务队列操作必须保证只有一个线程
    member = thpool->jobqueue_head;
    if(!member)
    {
        thpool->jobqueue_head = work;                          //如果此任务是第一个任务
    }
    else
    {
        while(member->next != NULL)                            //如果不是第一个任务就添加到最后
            member = member->next;
        member->next = work;
    }
    pthread_cond_signal(&thpool->jobqueue_cond);
    pthread_mutex_unlock(&thpool->jobqueue_mutex);
}

void thpool_create(int max_thr_num)                   //线程创建函数
{
    int i;   
    thpool = (thpool_t *)malloc(sizeof(thpool_t));    
    if(!thpool)
    {
        perror("malloc thpool error");
    }
    thpool->flag = 0;                                 //初始化的过程
    thpool->max_thr_num = max_thr_num;                
    thpool->thr_id = (pthread_t*)malloc(max_thr_num*sizeof(pthread_t));
    thpool->jobqueue_head = NULL;
    pthread_mutex_init(&thpool->jobqueue_mutex,NULL);
    pthread_cond_init(&thpool->jobqueue_cond,NULL);
    for(i = 0;i < max_thr_num;i++)
    {
        pthread_create(&thpool->thr_id[i],NULL,thread_routine,NULL);  //创建线程的过程
    }
}
void thpool_destroy()                            //销毁线程池
{
    printf("线程池正在销毁\n");
    int i;
    thpool_jobqueue *member;  
    if(thpool->flag != 0)                        //先判断一下销毁标志是否已经销毁
    {
        return;
    }
    thpool->flag = 1;                            //将销毁标志至为1，即需要销毁
    pthread_mutex_lock(&thpool->jobqueue_mutex); 
    pthread_cond_broadcast(&thpool->jobqueue_cond);  //广播所有线程要销毁的通知
    pthread_mutex_unlock(&thpool->jobqueue_mutex);   
    for(i = 0;i < thpool->max_thr_num;i++)          
    {
        pthread_join(thpool->thr_id[i],NULL);     //等待所有线程都结束
    }
        free(thpool->thr_id);                      //将所有线程ID释放
    while(thpool->jobqueue_head)
    {
        member = thpool->jobqueue_head;
        thpool->jobqueue_head = thpool->jobqueue_head->next;
        free(member);                              //释放每一个任务
    }
    pthread_mutex_destroy(&thpool->jobqueue_mutex);//销毁锁
    pthread_cond_destroy(&thpool->jobqueue_cond);  //销毁条件变量
    free(thpool);                                  //销毁指向线程池的指针
    printf("销毁完成\n");

}

void *func1()
{
    printf("thread %u is running\n",pthread_self());
    sleep(3);                                     //让别的线程有机会抢锁
}

int main(int argc,char *argv[])
{
    int i;

    thpool_create(5);                             //在线程池里创建5个线程
    for(i = 0;i < 10;i++)
    {
        thpool_add_task(func1,NULL);
    }
    sleep(20);
    thpool_destroy();
}
