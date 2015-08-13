/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Fri 07 Aug 2015 09:53:41 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<pthread.h>
#include"my_recv.c"

#define W_NAPS  'n'
#define R_NAPS  'y'
#define BUFFSIZE 1024
int       conn_fd;    //只需要一个conn_fd;

struct user 
{
    char username[10];
};

struct group
{
    char group_name[10];
    char admin_name[10];
    struct user member[10];
};
struct nf_friend
{
    struct user nf_friend[30];
}nf_friend;

struct regi_sign
{
    char flag;
    char username[10];
    char password[10];

};
struct users
{
    struct       user user;
    char         password[10];
    int          friend_num;
    struct user  friend[10];
    struct group group[10];
    struct users *next;
};



struct message 
{
    char         type[2];
    char         from[10];
    char         to[10];
    char         time[30];
    char         changefriname[10];
    struct       group group;
    char         news[500];
};
int              friend_num;  //好友数，全局变量
struct  user     user;        //存自己用户的名字
struct  user     friend[10];  
struct  user     member[10];
struct  group    group[3];


int my_input(char *buf,int len)      //自定义输入函数
{
    int i;
    int c;
    if(buf == NULL)
    {
        return -1;
    }
    i = 0;
    setbuf(stdin,NULL);
    while(((c = getchar()) != '\n' && (c != EOF) && (i < len-2)))
    {
        buf[i++] = c;
    }
    buf[i++] = '\n';
    buf[i++] = '\0';

    return 0;
}
int sign_in();
int apply_account();
void quit();
void * sign_func();
void login_menu();

void login_menu()
{
    printf("hello\n");
}
void *sign_func()
{}
void quit()             //客户端退出函数，目的是为了让服务器端释放套接字
{
    char quit_buf[BUFSIZE];
    struct regi_sign quit;
    memset(quit_buf,0,sizeof(quit_buf));
    memset(&quit,0,sizeof(struct regi_sign));
    quit.flag = 'q';
    memcpy(quit_buf,&quit,sizeof(struct regi_sign));
    if(send(conn_fd,quit_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("quit error",__LINE__);
    }
    close(conn_fd);
    exit(0);
}

int apply_account()       //申请账户函数
{
    char apply_buf[BUFSIZE];
    struct regi_sign apply;
    while(1)
    {
        memset(&apply,0,sizeof(struct regi_sign));
        apply.flag = 'r';
        printf("请输入用户名：");
        my_input(apply.username,10);
        printf("请输入密码：");
        my_input(apply.password,10);
        memset(apply_buf,0,sizeof(apply_buf));
        memcpy(apply_buf,&apply,sizeof(struct regi_sign));
        if(send(conn_fd,apply_buf,BUFFSIZE,0) != BUFSIZE)
        {
            my_err("send",__LINE__);
        }
        memset(apply_buf,0,sizeof(apply_buf));
        recv(conn_fd,apply_buf,sizeof(apply_buf),0);
        if(strcmp(apply_buf,"n") == 0)
        {
            printf("用户名已被使用，重新注册\n");
            return 0;
        }
        if(strcmp(apply_buf,"y") == 0)
        {
            printf("注册成功\n");
            return 1;
        }
        
    }
}
int sign_in()           //登录函数
{
    int              i;
    char apply_buf[BUFSIZE];
    struct regi_sign apply;
    while(1)
    {
        memset(&apply,0,sizeof(apply));
        apply.flag = 'l';
        printf("请输入用户名：");
        my_input(apply.username,10);
        printf("请输入密码：");
        my_input(apply.password,10);
        memset(apply_buf,0,sizeof(apply_buf));
        memcpy(apply_buf,&apply,sizeof(struct regi_sign));
        if(send(conn_fd,apply_buf,sizeof(apply_buf),0) != BUFSIZE)
        {
            my_err("send",__LINE__);
        }
        memset(apply_buf,0,sizeof(apply_buf));
        recv(conn_fd,apply_buf,sizeof(apply_buf),0);
        if(strcmp(apply_buf,"n") == 0)
        {
            printf("用户名或密码有错误\n");
            return 0;
        }
        if(strcmp(apply_buf,"y") == 0)
        {
            printf("登录成功\n");
            return 1;
        }
    }
}

int main(int argc,char *argv[])
{
    struct sockaddr_in      serv_addr;
    pthread_t               thid,thid1;
    int                     i;
    int                     ret;
    int                     serv_port;
    char                    recv_buf[BUFSIZE];
    char                     choice[20];        //用于登录初界面的选择

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    for(i = 1;i < argc;i++)
    {
        if(strcmp("-p",argv[i]) == 0)
        {
            serv_port = atoi(argv[i+1]);
            if(serv_port < 0||serv_addr.sin_port > 65535)
            {
                printf("invalid serv_addr.sin_port\n");
                exit(1);
            }
            else
            {
                serv_addr.sin_port = htons(serv_port);
            }
            continue;
        }
        if(strcmp("-a",argv[i]) == 0)
        {
            if(inet_aton(argv[i+1],&serv_addr.sin_addr) == 0)
            {
                printf("invalid server ip address\n");
                exit(1);
            }
            continue;
        }
    }
    if((conn_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        my_err("socket",__LINE__);
    }

    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in)) < 0)
    {
        my_err("connect",__LINE__);
    }
    //到这里所有的连接工作完成
    do
    {
        printf("\t\t\t1：注册\n");
        printf("\t\t\t2：登录\n");
        printf("\t\t\t0：退出\n");
        while(1)
        {  
            printf("请选择(0 ~ 2)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2")))
                break;    
            else
             {
                 printf("\n无效输入，请重新输入\n");
             }
        }
        
        switch(atoi(choice))
        {
            case 1:
            if((ret = apply_account()) == 1)
            {
                strcpy(choice,"-1");
                break;
            }
            else
            {
                break;
            }
            case 2:
            if((ret = sign_in()) == 1)
            {
                strcpy(choice,"-1");
                break;
            }
            else
                break;
            case 0:
            quit();
        }
    }while(strcmp(choice,"-1"));
    pthread_create(&thid,NULL,sign_func,NULL);
    login_menu();
    close(conn_fd);
    return 0;
}
