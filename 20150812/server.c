/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Fri 07 Aug 2015 08:45:39 PM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include"my_recv.c"
#define SERV_PORT        4507
#define LISTENQ          30
#define BUFSIZE          1024
#define LEN     sizeof(struct users)

pthread_mutex_t          mutex;
struct user
{
    char username[10];
};
struct regi_sign
{
    char flag;
    char username[10];
    char password[10];
};
struct group
{
    char group_name[10];
    char build_name[10];
    struct user member[10]; 
};
struct connect_info
{
    int fd;
    char name[10];
}connect_info[30];

struct nf_friend   //附近的人和查看好友
{
    struct user nf_friend[30];
}nf_friend;
struct users
{
    struct user user;          //用户名
    char password[10];         //密码
    int  friend_num;           //朋友数
    struct user friend[10];    //朋友信息
    struct group group[10];    //群
    struct users *next;
};

struct users*  head = NULL;
struct users*  p;

struct message 
{
    char      type[2];
    char      from[10];
    char      to[10];
    char      time[30];
    char      changefriname[10];  //添加好友和删除
    struct    group group;
    char      news[500];
};
struct users* read_input();
struct users* apply_account(struct regi_sign,int);
void save();
char * my_time();
struct users* sign_in(struct regi_sign,int);
void quit(struct regi_sign,int);

void quit(struct regi_sign account,int i)
{
    struct regi_sign quit;
    quit = account;
    pthread_mutex_unlock(&mutex);
    connect_info[i].fd = -1;
    printf("[用户]%s下线了 \t%s\n",quit.username,my_time());
    pthread_exit(0);
}
struct users* sign_in(struct regi_sign account,int i)
{
    struct regi_sign  sign;
    struct users  *p1;
    p1 = head->next;
    sign = account;
    while(p1)
    {
        if((!strcmp((p1->user).username,sign.username)) && (!strcmp(p1->password,sign.password)))
        {
            send(connect_info[i].fd,"y",BUFSIZE,0);
            printf("[用户]%s上线了 \t%s\n",sign.username,my_time());
            
            return p1;
        }
        p1 = p1->next;
    }
    if(p1 == NULL)
    {
        send(connect_info[i].fd,"n",BUFSIZE,0);
        return NULL;
    }
}
char * my_time()
{
    time_t now;
    time(&now);
    return (ctime(&now));
}
void save()
{
    FILE *fp;
    struct users *p1;
    p1 = head->next;
    if((fp = fopen("userlist","wb")) == NULL)
    {
        my_err("fopen",__LINE__);
        exit(0);
    }
    while(p1 != NULL)
    {
        fwrite(p1,LEN,1,fp);
        p1 = p1->next;
    }
    fclose(fp);

}
struct users* apply_account(struct regi_sign account,int i)
{
    struct    regi_sign apply;
    struct    users   *p1,*p2;
    p1 = head;
    apply = account;
    while(p1->next != NULL)
    {
        if(strcmp((p1->next->user).username,apply.username) == 0)
        {
            send(connect_info[i].fd,"n",BUFSIZE,0);
            return NULL;
        }
        p1 = p1->next;
    }
    p2 = (struct users*)malloc(LEN);
    strcpy((p2->user).username,apply.username);
    strcpy(p2->password,apply.password);
    p1->next = p2;
    p2->next = NULL;
    save();
    send(connect_info[i].fd,"y",1024,0);
    printf("[用户]%s注册成功 \t%s\n",apply.username,my_time());
    return (p2);
}


struct users* read_input()
{
    FILE *fp;
    struct users *p1,*p2;
    int    ret;
    if((fp = fopen("userlist","rb")) == NULL)
    {
        my_err("fopen",__LINE__);
        exit(0);
    }
    p1 = (struct users *)malloc(LEN);
    p2 = (struct users *)malloc(LEN);
    head = p1;
    fread(p2,LEN,1,fp);
    while(!feof(fp))
    {
        p1->next = p2;
        p1 = p2;
        p2 = (struct users*)malloc(LEN);
        fread(p2,LEN,1,fp);
    }
    p1->next = NULL;
    free(p2);
    fclose(fp);
    return (head);
}



void *client_t(void *arg)
{
    int i = *(int *)arg;
    char      recv_buf[BUFSIZE];
    char      flag[BUFSIZE];
    struct    user* p_user;
    while(1)
    {
        struct    regi_sign account;
        memset(&account,0,sizeof(struct regi_sign));
        memset(flag,0,sizeof(flag));
        memset(recv_buf,0,sizeof(recv_buf));
        if(recv(connect_info[i].fd,recv_buf,sizeof(recv_buf),0) != BUFSIZE)
        {
            my_err("recv",__LINE__);
            pthread_exit(0);
        }
        pthread_mutex_lock(&mutex);
        memcpy(&account,recv_buf,sizeof(recv_buf));
        head = read_input();
        if(head)
        if(account.flag == 'r')  //注册
        {
            p = apply_account(account,i);
            pthread_mutex_unlock(&mutex);
            if(p != NULL)
                break;
        }
        if(account.flag == 'l')  //登录
        {
            p = sign_in(account,i);
            pthread_mutex_unlock(&mutex);
            if(p != NULL)
                break;
            }
        if(account.flag == 'q')  //退出
        {
            quit(account,i);
        }
    }
}


int main(int argc,char *argv[])
{
    int                          sock_fd,conn_fd;
    struct     sockaddr_in       serv_addr,conn_addr;
    socklen_t                    conn_len;
    pid_t                        pid;
    char                         recv_buf[128];
    char                         chat_buf[128];
    int                          ret,i;
    int                          optval;
    time_t                       now;
    pthread_t                    thid,quit_thid;
    
    pthread_mutex_init(&mutex,NULL);
    if((sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        my_err("socket",__LINE__);
    }

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) < 0)
    {
        my_err("bind",__LINE__);
    }
    if(listen(sock_fd,LISTENQ) < 0)
    {
        my_err("bind",__LINE__);
    }
    for(i = 0;i < 30;i++)
    {
        connect_info[i].fd = -1;
        strcpy(connect_info[i].name," ");
    }
    conn_len = sizeof(struct sockaddr_in);
    while(1)
    {
        for(i = 0;i < 30;i++)
        {
            if(connect_info[i].fd == -1)
                break;
        }
        if((conn_fd= accept(sock_fd,(struct sockaddr *)&conn_addr,&conn_len)) < 0)
        {
            my_err("accept",__LINE__);
        }
        connect_info[i].fd = conn_fd;
        time(&now);
        printf("有新的连接：ip:%s\t%s\n",inet_ntoa(conn_addr.sin_addr),ctime(&now));
        int f = i;
        pthread_create(&thid,NULL,client_t,(void*)&f);
    }
     return 0;

}
