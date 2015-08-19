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
#include"inputkey_getch.h"

#define W_NAPS  'n'
#define R_NAPS  'y'
#define BUFSIZE 1024
#define LEN1      sizeof(struct message)
int       conn_fd;     //只需要一个conn_fd
int       b=0;         //添加好友请求数
int       c=10;        //申请入群数
int       d=20;
pthread_mutex_t   mutex;
struct user 
{
    char username[10];                 //姓名
};

struct group                           //群组
{
    char group_name[10];
    char admin_name[10];
    struct user member[10];
    int  member_num;
};

struct regi_sign                       //注册登录结构体
{
    char flag;
    char username[10];
    char password[10];

};
struct users                           //用户信息结构体
{
    struct       user user;
    char         password[10];
    int          friend_num;
    int          group_num;
    struct user  friend[10];
    struct group group[10];
    struct users *next;
};



struct message                         //消息结构体
{
    char         type;
    char         from[10];
    char         to[10];
    char         time[30];
    char         changefriname[10];
    struct       user name[30];
    struct       group group;
    char         news[500];
};
int              friend_num;           //好友数，全局变量
char             myname[10];           //存自己用户的名字
struct  user     friend[10];         
struct  user     member[10];
struct  group    group[3];
struct message   chat_t[30];           //加好友，离线消息，群管理

char * my_time()                       //自定义时间函数
{
    time_t  now;
    time(&now);
    return (ctime(&now));
}

int my_input(char *buf,int len)        //自定义输入函数
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
    buf[i++] = '\0';
    buf[i++] = '\n';

    return 0;
}
int sign_in();        //登录
int apply_account();  //申请账户
void quit();          //登录界面退出
void * sign_func();   //线程处理函数 
void login_menu();    //登录菜单
int near_friend();    //附近的人
int admin_friend();   //好友管理
int view_friend();    //查看所有好友
int view_online_friend();//在线好友
int add_friend();     //添加好友
void out_line();      //下线
void del_friend();    //删除好友
int chat_with();      //聊天函数
int pri_chat();       //私聊
int build_group();    //建群
int view_group();     //查看群
int public_chat();    //群聊
void view_chat_record();//查看聊天记录  
int message_ment();   //消息管理
void add_friend_req();//添加好友请求
void accept_add(int);//接受添加
void apply_add_group();//申请入群

void apply_add_group()
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'h';
    memcpy(near.from,myname,10);
    printf("请输入想要加入群聊的群名:");
    my_input(near.to,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
    }

}
void accept_add(int k) //接受添加
{
    int t;
    char choice;
    struct message chat;
    chat = chat_t[k];
    printf("[系统提示]用户 %s 想添加您为好友，同意输入y,不同意输入n\n",chat.from);
    setbuf(stdin,NULL);
    scanf("%c",&choice);
    switch(choice)
    {
        case 'y':
        {
            char near_buf[BUFSIZE];
            memset(near_buf,0,BUFSIZE);
            chat.type = 't';
            memcpy(near_buf,&chat,LEN1);
            if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
            {
                my_err("send",__LINE__);
            }
            printf("[用户] %s，已经成为您的好友了\n",chat.from);
            break;
        }
        case 'n':
        {
            printf("\n");
            char near_buf[BUFSIZE];
            memset(near_buf,0,BUFSIZE);
            chat.type = 'e';
            memcpy(near_buf,&chat,LEN1);
            if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
            {
                my_err("send",__LINE__);
            }
            printf("您拒绝了用户 %s 的添加好友请求\n",chat.from);
            break;
        }
    }
    for(t = k;t < 10;t++)
    {
        chat_t[t] = chat_t[t+1]; //之后将我的消息提醒向前挪
    }
        b--;
}
void add_friend_req()   //好友添加消息通知的显示
{
    int k,t,choice;
    system("clear");
    printf("-----------------------------------------\n");
    for(k = 0,t = 1;k < b;k++,t++)
    {
        printf("[序号]%d 用户“%s” 想添加您为好友\n",t,chat_t[k].from);
    }
    printf("\n\n\n\n\n");
    printf("请输入要处理的序号：");
    scanf("%d",&choice);
    accept_add(choice-1);
}
void accept_add_group(int k)
{
    int t;
    char choice;
    struct message chat;
    chat = chat_t[k];
    printf("[系统提示]用户 %s 想申请加入群“%s”，同意输入y,不同意输入n\n",chat.from,chat.to);
    printf("群主：%s\n",chat.group.admin_name);
    setbuf(stdin,NULL);
    scanf("%c",&choice);
    switch(choice)
    {
        case 'y':
        {
            char near_buf[BUFSIZE];
            memset(near_buf,0,BUFSIZE);
            chat.type = 'a';
            memcpy(near_buf,&chat,LEN1);
            if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
            {
                my_err("send",__LINE__);
            }
            printf("[用户] %s，已经进入群%s\n",chat.from,chat.to);
            break;
        }
        case 'n':
        {
            printf("\n");
            char near_buf[BUFSIZE];
            memset(near_buf,0,BUFSIZE);
            chat.type = 'i';
            memcpy(near_buf,&chat,LEN1);
            if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
            {
                my_err("send",__LINE__);
            }
            printf("您拒绝了用户 %s 的入群请求\n",chat.from);
            break;
        }
    }
    for(t = k;t < 20;t++)
    {
        chat_t[t] = chat_t[t+1]; //之后将我的消息提醒向前挪
    }
        c--;

}
void add_group_req()
{
    int k,t,choice;
    system("clear");
    printf("-----------------------------------------\n");
    for(k = 10,t = 1;k < c;k++,t++)
    {
        printf("[序号]%d 用户“%s” 想申请加入群“%s”\n",t,chat_t[k].from,chat_t[k].to);
    }
    printf("\n\n\n\n\n");
    printf("请输入要处理的序号：");
    scanf("%d",&choice);
    accept_add_group(choice+9);
}

int message_ment()      //通知类消息管理
{
    char choice[20];
    system("clear");
    do
    {
        printf("1:添加好友请求[%d]\n",b);
        printf("2:申请入群请求[%d]\n",c-10);
        printf("3:文件发送请求\n");
        printf("0:返回上级菜单\n");
        while(1)
        {  
            printf("请选择(0 ~ 3)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2"))||(!strcmp(choice,"3")))
                break;    
            else
             {
                 printf("\n无效输入，请重新输入\n");
             }
        }
        switch(atoi(choice))
        {
            case 1:
            {
                add_friend_req();
                break;
            }
            case 2:
            {
                add_group_req();
                break;
            }
            case 3:
            {
                break;
            }
            
            case 0:
            {
                return 0;
            }
        }
    }while(strcmp(choice,"-1"));

}
void view_chat_record()     //查看聊天记录
{
    struct message near;
    memset(&near,0,LEN1);
    char name[10];
    FILE *fp;
    printf("请输入想要查看聊天记录的好友或者群名称:\n");
    my_input(name,10);
    if((fp = fopen(name,"r")) == NULL)
    {
        printf("未找到您说的好友或群\n");
        return;
    }
    while((fread(&near,LEN1,1,fp) != -1) && (!feof(fp))) //找到相应的文件读出内容
    {
        printf("%s %s 说 %s\n ",near.time,near.from,near.news);
    }
    fclose(fp);
}
int public_chat()                        //群聊
{
    char   near_buf[BUFSIZE];
    struct message near;
    FILE   *fp;

    memset(&near,0,LEN1);                //群聊消息结构体的初始化 
    memset(near_buf,0,BUFSIZE);
    near.type = 'b';
    memcpy(near.from,myname,10);
    printf("请输入群名\n");
    my_input(near.to,10);
    memcpy(near.time,my_time(),30);
    system("clear");
    printf("-------你正在群 %s 里聊天，谨防诈骗！(如果想退出，输入quit)-------\n",near.to);
    while(1)
    {
        printf("%s",my_time());
        printf("%s说：",near.from);
        memset(near.news,0,100);
        my_input(near.news,100);
        if((fp = fopen(near.to,"ab+")) == NULL)  //没有文件就自己创建文件
        {
                my_err("fopen",__LINE__);
                exit(0);
        }
        fwrite(&near,LEN1,1,fp);
        fclose(fp);
        if(strcmp(near.news,"quit") == 0)        //当输入quit,表示退出
        {
            return 0;
        }
        memcpy(near_buf,&near,LEN1);
        if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
        {
            my_err("send",__LINE__);
            exit(0);
        }
    }


}
int view_group()                                 //查看自己的群的信息
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'c';
    memcpy(near.from,myname,10);
    printf("请输入要查看的群名:");
    my_input(near.to,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }

}
int build_group()                                //建群
{
    int    i,k;
    int    number;
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);                        //对群的消息的结构体初始化
    memset(near_buf,0,BUFSIZE);
    near.type = 'j';
    memcpy(near.from,myname,10);
    memcpy(near.time,my_time(),30);
    printf("请输入要建立的群名称:\n");
    my_input(near.group.group_name,10);
    printf("请输入群成员个数(0-9):");
    scanf("%d",&number);
    memcpy(&near.group.member_num,&number,4);
    printf("请输入群成员名称\n");
    for(i = 0,k = 1;i < number;i++,k++)
    {
        printf("第%d个成员\n",k);
        my_input(near.group.member[i].username,10);
    }
    memcpy(near.group.admin_name,myname,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }

}
int pri_chat()                                   //私聊
{
    char   near_buf[BUFSIZE];
    struct message near;
    FILE   *fp;
 
    memset(&near,0,LEN1);                        //私聊消息初始化
    memset(near_buf,0,BUFSIZE);
    near.type = 'o';
    memcpy(near.from,myname,10);
    printf("请输入您私聊的好友姓名\n");
    my_input(near.to,10);
    memcpy(near.time,my_time(),30);
    system("clear");
    printf("-------你正在与 %s 聊天(如果想退出，输入quit)-------\n",near.to);
    while(1)
    {
        printf("%s",my_time());                  //只要不输入quit就一直让它循环
        printf("%s说：",near.from);
        memset(near.news,0,100);
        my_input(near.news,100);
        if((fp = fopen(near.to,"ab+")) == NULL)
        {
                my_err("fopen",__LINE__);
                exit(0);
        }
        fwrite(&near,LEN1,1,fp);
        fclose(fp);
        if(strcmp(near.news,"quit") == 0)
        {
            return 0;
        }
        memcpy(near_buf,&near,LEN1);
        if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
        {
            my_err("send",__LINE__);
            exit(0);
        }
    }

}
int chat_with()
{
    char   choice[20];
    system("clear");
    do
    {
        printf("1:我要私聊\n");
        printf("2:查看我的群\n");
        printf("3:我要建群\n");
        printf("4:申请入群\n");
        printf("5:我要群聊\n");
        printf("6:消息管理\n");
        printf("0:返回上级菜单\n");
        while(1)
        {  
            printf("请选择(0 ~ 6)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2"))||(!strcmp(choice,"3")) || (!strcmp(choice,"4"))||(!strcmp(choice,"5"))||(!strcmp(choice,"6")))
                break;    
            else
             {
                 printf("\n无效输入，请重新输入\n");
             }
        }
        switch(atoi(choice))
        {
            case 1:
            {
                pri_chat();
                break;
            }
            case 2:
            {
                view_group();
                break;
            }
            case 3:
            {
                build_group();
                break;
            }
            case 4:
            {
                apply_add_group();
                break;
            }
            case 5:
            {
                public_chat();
                break;
            }
            case 6:
            {
                message_ment();
                break;
            }
            case 0:
            {
                return 0;
            }
        }
    }while(strcmp(choice,"-1"));

}
void del_friend()                   //删除好友需要将好友信息存到消息结构体中       
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 's';
    memcpy(near.from,myname,10);
    printf("请输入要删除的好友姓名\n");
    my_input(near.to,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }

}
void out_line()                     //下线，
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'g';
    memcpy(near.from,myname,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }
    close(conn_fd);                 //close 掉conn_fd
    exit(0);

}
int add_friend()                     //添加好友第一次请求
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'y';
    memcpy(near.from,myname,10);
    printf("请输入要添加的好友姓名\n");
    my_input(near.to,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }

}
int view_online_friend()            //查看在线好友
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'z';
    memcpy(near.from,myname,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }
}


int view_friend()                   //查看所有好友
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'v';
    memcpy(near.from,myname,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }
}

int admin_friend()                  //好友管理函数
{
    char   choice[20];
    system("clear");
    do
    {
        printf("1:查看所有好友\n");
        printf("2:查看在线好友\n");
        printf("3:添加好友\n");
        printf("4:删除好友\n");
        printf("5:消息管理\n");
        printf("0:返回上级菜单\n");
        while(1)
        {  
            printf("请选择(0 ~ 5)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2"))||(!strcmp(choice,"3")) || (!strcmp(choice,"4")) || (!strcmp(choice,"5")))
                break;    
            else
             {
                 printf("\n无效输入，请重新输入\n");
             }
        }
        switch(atoi(choice))
        {
            case 1:
            {
                view_friend();
                break;
            }
            case 2:
            {
                view_online_friend();
                break;
            }
            case 3:
            {
                add_friend();
                break;
            }
            case 4:
            {
                del_friend();
                break;
            }
            case 5:
            {
                message_ment();
                break;
            }
            case 0:
            {
                return 0;
            }
        }

    }while(strcmp(choice,"-1"));
}
int near_friend()            //附近的人
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'f';
    memcpy(near.from,myname,10);
    memcpy(near_buf,&near,LEN1);
    if(send(conn_fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        exit(0);
    }
}
void login_menu()            //登录主菜单
{
    char     choice[20];
    struct   message near;
    system("clear");
    do
    {
        printf("\t\t %s\n",myname);
        printf("1:附近的人\n");
        printf("2:好友管理\n");
        printf("3:我要聊天\n");
        printf("4:查看聊天记录\n");
        printf("5:消息管理\n");
        printf("0:我要下线\n");
        while(1)
        {  
            printf("请选择(0 ~ 5)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2"))||(!strcmp(choice,"3")) || (!strcmp(choice,"4")) || (!strcmp(choice,"5")))
                break;    
            else
             {
                 printf("\n无效输入，请重新输入\n");
             }
        }
        switch(atoi(choice))
        {
            case 1:
            {
                near_friend();
                break;
                
            }
            case 2:
            {
                admin_friend();
                break;
            }
            case 3:
            {
                chat_with();
                break;
            }
            case 4:
            {
                view_chat_record();
                break;
            }
            case 5:
            {
                message_ment();
                break;
            }
            case 0:
            {
                out_line();
            }
        }
    }while(strcmp(choice,"-1"));
}
void *sign_func()
{
    while(1)
    {
        struct message  chat;
        char   recv_buf[BUFSIZE];
        memset(&chat,0,LEN1);
        memset(recv_buf,0,BUFSIZE);
        sleep(1);
        if(recv(conn_fd,recv_buf,BUFSIZE,0) != BUFSIZE)
        {
            my_err("服务器退出",__LINE__);
            pthread_exit(0);
        }
        memcpy(&chat,recv_buf,BUFSIZE);
        //printf("chat.type:%c\n",chat.type);
        switch(chat.type)
        {
            case 'f'://附近的人
            {
                int i;
                printf("\n");
                printf("---------------------\n");
                printf("他们都在线，可以加好友或者找他们聊天哦，打个招呼吧！\n");
                for(i = 0;i < 30;i++)
                {
                    fflush(stdout);
                    if(strcmp(chat.name[i].username,chat.from))
                        printf("%s\t",chat.name[i].username);
                }
                printf("\n");
                break;
            }
            case 'v'://查看所有好友
            {
                int i ;
                printf("\n");
                printf("这是您的全部好友\n");
                for(i = 0;i < 10;i++)
                {
                    fflush(stdout);
                    if(strcmp(chat.name[i].username,chat.from))
                        printf("%s\t",chat.name[i].username);
                }
                printf("\n");
                break;
            }
            case 't'://添加好友被同意
            {
                printf("\n");
                printf("[用户]%s 同意了您的添加好友请求，已经成为您的好友了\n",chat.to);
                break;
            }
            case 'z'://查看在线好友
            {
                int i;
                printf("\n");
                printf("这是您的在线好友\n");
                for(i = 0;i < 10;i++)
                {
                    fflush(stdout);
                    if(strcmp(chat.name[i].username,chat.from))
                        printf("%s\t",chat.name[i].username);
                }
                printf("\n");
                break;
            }
            case 's'://删除好友
            {
                printf("\n");
                printf("[用户]%s 已经被您删除\n",chat.to);
                break;
            }
            case 'o'://私聊
            {
                FILE *fp;
                if((fp = fopen(chat.from,"ab+")) == NULL)
                {
                    my_err("fopen",__LINE__);
                    exit(0);
                }
                fwrite(&chat,LEN1,1,fp);
                fclose(fp);
                printf("\n");
                printf("%s",my_time());
                printf("[私]%s 说：%s\n",chat.from,chat.news);
                break;
            }
            case 'j'://建群  
            {
                printf("\n");
                printf("%s",my_time());
                printf("[系统提示] 群‘%s’ 已经建立成功\n",chat.group.group_name);
                break;
            }
            case 'c'://查看群信息
            {
                int k;
                printf("\n");
                fflush(stdout);
                printf("群名：%s\n",chat.group.group_name);
                printf("群主：%s\n",chat.group.admin_name);
                printf("群成员：");
                for(k = 0;k < 10;k++)
                {
                    fflush(stdout);
                    printf("%s  ",chat.group.member[k].username);
                }
                printf("\n");
                break;
            }
            case 'b'://群消息通知
            {
                FILE *fp;
                if((fp = fopen(chat.to,"ab+")) == NULL)
                {
                    my_err("fopen",__LINE__);
                    exit(0);
                }
                fwrite(&chat,LEN1,1,fp);
                fclose(fp);
                printf("\n");
                printf("%s",chat.time);
                printf("[群：%s]%s 说：%s\n",chat.to,chat.from,chat.news);
                break;
            }
            case 'd'://离线消息
            {
                printf("\n");
                printf("好友 %s 离线了，你现在发送的消息是离线消息。\n",chat.to);
                break;
            }
            case 'm'://添加好友失败的原因
            {
                printf("\n");                
                printf("对不起，未找到 %s 用户，或者他不在线\n",chat.to);
                break;
            }
            case 'y'://添加好友请求提醒            
            {
                printf("\n");
                printf("[系统提示]用户 %s 想添加您为好友，请到“消息管理-添加好友请求”处理\n",chat.from);
                if(b <= 9)
                    chat_t[b++] = chat;
                break;
            }
            case 'e'://添加好友被拒绝
            {
                printf("\n");
                printf("[系统提示]：用户 %s 拒绝了您的添加好友请求\n",chat.to);
                break;
            }
            case 'x'://离线消息的收取
            {
                FILE *fp;
                if((fp = fopen(chat.from,"ab+")) == NULL)
                {
                    my_err("fopen",__LINE__);
                    exit(0);
                }
                fwrite(&chat,LEN1,1,fp);
                fclose(fp);
                printf("\n");
                printf("%s",my_time());
                printf("[离线]%s 说：%s\n",chat.from,chat.news);
                break;
            }
            case 'r'://进群通知
            {
                printf("\n");
                printf("[系统通知] 用户“%s”邀请您加入了群“%s”\n",chat.from,chat.group.group_name);
                break;
            }
            case 'h':
            {
                printf("\n");
                printf("[系统提示]用户 %s 想申请加入群“%s”，请到“消息管理-申请入群请求”处理\n",chat.from,chat.to);
                if(c <= 19)
                    chat_t[c++] = chat;
                break;
            }
            case 'a':
            {
                printf("\n");
                printf("[系统提示] 群主“%s”同意了您加入群“%s”的请求,快去群里和大家打个招呼吧！\n",chat.group.admin_name,chat.to);
                break;
            }
            case 'i':
            {
                printf("\n");
                printf("[系统提示] 群主“%s”拒绝了您加入群“%s”的请求\n",chat.group.admin_name,chat.to);
                break; 
            }
            case '1'://删除失败
            {
                printf("\n");
                printf("对不起，您没有此好友。请查证后在删除\n");
                break;
            }
            case '2'://添加自己
            {
                printf("\n");
                printf("对不起，您不能把自己添加为好友\n");
                break;
            }
            case '3'://删除自己
            {
                printf("\n");
                printf("对不起，您不能删除自己\n");
                break;
            }
            case '4'://再次添加
            {
                printf("\n");
                printf("“%s”已经是您的好友了\n",chat.to);
                break;
            }
            case '5'://私聊没此人
            {
                printf("\n");
                printf("对不起，无此用户\n");
                break;
            }
            case '6'://和自己聊天
            {
                printf("\n");
                printf("到底是多孤单，才会和自己聊天,去附近的人看看吧！\n");
                break;
            }

        }
    }

}
void quit()             //客户端退出函数，目的是为了让服务器端释放套接字
{
    int ret;
    char quit_buf[BUFSIZE];
    struct regi_sign quit;
    memset(quit_buf,0,sizeof(quit_buf));
    memset(&quit,0,sizeof(struct regi_sign));
    quit.flag = 'q';
    memcpy(quit.username,myname,sizeof(myname));
    memcpy(quit_buf,&quit,sizeof(struct regi_sign));
    printf("%s\n",quit_buf);
    if((ret = send(conn_fd,quit_buf,BUFSIZE,0)) != BUFSIZE)
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
        if(strlen(apply.username) == 0)
        {
            printf("用户名不合法\n");
            return 0;
        }
        printf("请输入密码：");
        inputkey(apply.password);
        memset(apply_buf,0,sizeof(apply_buf));
        memcpy(apply_buf,&apply,sizeof(struct regi_sign));
        if(send(conn_fd,apply_buf,BUFSIZE,0) != BUFSIZE)
        {
            my_err("send",__LINE__);
        }
        memset(apply_buf,0,sizeof(apply_buf));
        recv(conn_fd,apply_buf,sizeof(apply_buf),0);
        if(strcmp(apply_buf,"n") == 0)
        {
            printf("\n");
            printf("用户名已被使用，重新注册\n");
            return 0;
        }
        if(strcmp(apply_buf,"y") == 0)
        {
            printf("\n");
            printf("注册成功\n");
            memset(myname,0,sizeof(struct user));
            memcpy(myname,apply.username,sizeof(apply.username));
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
        inputkey(apply.password);
        memset(apply_buf,0,sizeof(apply_buf));
        memcpy(apply_buf,&apply,sizeof(struct regi_sign));
        if(send(conn_fd,apply_buf,sizeof(apply_buf),0) != BUFSIZE)
        {
            my_err("send",__LINE__);
        }
        memset(apply_buf,0,sizeof(apply_buf));
        if((recv(conn_fd,apply_buf,sizeof(apply_buf),0)) != BUFSIZE)
        {
            my_err("recv",__LINE__);
            exit(0);
        }
        if(strcmp(apply_buf,"n") == 0)
        {
            printf("\n");
            printf("用户名或密码有错误\n");
            return 0;
        }
        if(strcmp(apply_buf,"y") == 0)
        {
            printf("\n");
            printf("登录成功\n");
            memset(myname,0,sizeof(struct user));
            memcpy(myname,apply.username,sizeof(apply.username));
            return 1;
        }
        if(strcmp(apply_buf,"w") == 0)
        {
            printf("\n");
            printf("此用户已经在线，拒绝登录\n");
            return 0;
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
    pthread_mutex_init(&mutex,NULL);
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
                break;
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
    return 0;
}
