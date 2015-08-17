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
#define LEN1    sizeof(struct message)

pthread_mutex_t          mutex;
int                      y=0;  //分配离线消息的结构体
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

struct users
{
    struct user user;          //用户名
    char password[10];         //密码
    int  friend_num;           //朋友数
    int  group_num;            //群数
    struct user friend[10];    //朋友信息
    struct group group[10];    //群
    struct users *next;
};
struct leave_message
{
    char  name[10];
    int   number;
}leave_message[30];

struct users*  head = NULL;
struct users*  p;

struct message 
{
    char      type;
    char      from[10];
    char      to[10];
    char      time[30];
    char      changefriname[10];//添加好友和删除
    struct    user name[30]; //附近的人，在线好友。查看好友
    struct    group group;
    char      news[500];
};
struct users* read_input();
struct users* apply_account(struct regi_sign,int);
void save();
char * my_time();
struct users* sign_in(struct regi_sign,int);
void quit(struct regi_sign,int);
void near_friend(struct message,int);
void view_friend(struct message,int);
void add_friend(struct message,int);
void out_line(struct message,int);
void view_online_friend(struct message,int);
void del_friend(struct message,int);
int pri_chat(struct message,int);
void build_group(struct message,int);
void view_group(struct message,int);
void public_chat(struct message,int);
void leave_message_func(struct regi_sign,int);

void leave_message_func(struct regi_sign account,int i)
{
    struct message chat;
    char   near_buf[BUFSIZE];
    int k,j;
    FILE *fp;
    memset(&chat,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    printf("11111111111-------\n");
    printf("%s %s\n",account.username,leave_message[0].name);
    printf("leave_message[0].number:%d\n",leave_message[0].number);
    for(k = 0;k < 30;k++)
    {
        if(!(strcmp(account.username,leave_message[k].name)))
        {
            if(leave_message[k].number != 0)
            {
                printf("2222222222222");
                if((fp = fopen(leave_message[k].name,"a+")) == NULL)
                {
                    my_err("fopen",__LINE__);
                    exit(0);
                }
                while((fread(&chat,LEN1,1,fp) != -1) && (!feof(fp)))
                {
                    chat.type = 'x';
                    printf("chat,from:%s\n",chat.from);
                    memcpy(near_buf,&chat,LEN1);
                    if(send(connect_info[i].fd,near_buf,BUFSIZE,0)!=BUFSIZE)
                    {
                        my_err("send",__LINE__);
                        pthread_exit(0);
                    }
                }
                fclose(fp);
                leave_message[k].number = 0;
                unlink(leave_message[k].name);
                
            }
        }
    }

}
void public_chat(struct message chat ,int i)
{
    int k,j;
    struct users *p1;
    p1 = head->next;
    char  near_buf[BUFSIZE];
    struct message near;
    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near = chat;
    while(p1)
    {
        if(!strcmp(near.from,(p1->user).username))
        {
            for(k = 0;k < 10;k++)
            {
                if(!strcmp((p1->group)[k].group_name,near.to))
                {
                    for(j = 0;j < 30;j++)
                    {
                        if((!strcmp(connect_info[j].name,(p1->group)[k].member[j].username)) && (connect_info[j].fd != connect_info[i].fd))
                       // if(!strcmp(connect_info[j].name,(p1->group)[k].member[j].username))
                        {
                            memcpy(near_buf,&near,LEN1);
                            if(send(connect_info[j].fd,near_buf,BUFSIZE,0) != BUFSIZE)
                            {
                                my_err("send",__LINE__);
                                pthread_exit(0);
                            }
                        }

                    }
                }
            }
        }
        p1 = p1->next;
    }
printf("111111\n");
}
void view_group(struct message chat,int i)
{
    int k ;
    struct users *p1;
    p1 = head->next;
    char  near_buf[BUFSIZE];
    struct message near;
    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near = chat;
    while(p1)
    {
        if(!strcmp(near.from,(p1->user).username))
        {
            for(k = 0;k < 10;k++)
            {
               if(!strcmp((p1->group)[k].group_name,near.to))
                    near.group = (p1->group)[k];
            }
        }
        p1 = p1->next;
    }
    memcpy(near_buf,&near,LEN1);
    if(send(connect_info[i].fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        pthread_exit(0);
    }

}
void build_group(struct message chat,int i)
{
    int k ;
    struct users *p1;
    p1 = head->next;
    char  near_buf[BUFSIZE];
    struct message near;
    struct user  name[10];
    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near = chat;
    for(k = 0; k < 10; k++)
    {
        memcpy(name[k].username,near.group.member[k].username,10);
    }
    while(p1)
    {
        for(k = 0;k < 10;k++)
        {
            if(!(strcmp(name[k].username,(p1->user).username)))
            {
                (p1->group)[p1->group_num] = near.group;
                (p1->group_num)++;
            }
        }
        p1 = p1->next;
    }
    save();
    memcpy(near_buf,&near,LEN1);
    if(send(connect_info[i].fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        pthread_exit(0);
    }

    
}
int pri_chat(struct message chat,int i)
{
    FILE *fp;
    int  k;
    struct  message  near;
    char    near_buf[BUFSIZE];
    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near = chat;
    for(k = 0;k < 30;k++)
    {
        if(!strcmp(near.to,connect_info[k].name))
        {
            near.type = 'o';
            memcpy(near_buf,&near,LEN1);
            if(send(connect_info[k].fd,near_buf,BUFSIZE,0) != BUFSIZE)
            {
                my_err("send",__LINE__);
                pthread_exit(0);
            }
            return 0;
        }
    }
    if(k == 30)
    {
        near.type = 'd';
        memcpy(near_buf,&near,LEN1);
        if(send(connect_info[i].fd,near_buf,BUFSIZE,0) != BUFSIZE)
        {
            my_err("send",__LINE__);
            pthread_exit(0);
        }
        if((fp = fopen(near.to,"ab+")) == NULL)
        {
            my_err("fopen",__LINE__);
            exit(0);
        }
        fwrite(&near,LEN1,1,fp);
        fclose(fp);
        printf("------------------near.to:%s,%s\n",near.to,leave_message[0].name);
        for(k = 0;k < 30;k++)
        {
            if(!(strcmp(leave_message[k].name,near.to)))
                leave_message[k].number = leave_message[k].number+1;
        }


    }
}
void del_friend(struct message chat,int i)
{
    struct  users *p1;
    p1 = head->next;
    int k;
    struct message near;
    memset(&near,0,LEN1);
    near = chat;
    char  near_buf[BUFSIZE];
    while(p1)
    {
        if(!(strcmp((p1->user).username,near.from)))
        {
            for(k = 0;k < 10;k++)
            {
                printf("%s\n",(p1->friend[k].username));
                if(!(strcmp(near.to,(p1->friend)[k].username)))
                {
                    for(k;k < 10;k++)
                    {
                        memcpy((p1->friend)[k].username,(p1->friend)[k+1].username,10);
                    }
                    break;
                    p1->friend_num--;
                }
            }
            
        }        
        p1 = p1->next;
    }
    save();
    memset(near_buf,0,BUFSIZE);
    memcpy(near_buf,&near,LEN1);
    if(send(connect_info[i].fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        pthread_exit(0);
    }
}

void view_online_friend(struct message chat,int i)
{
    struct  users *p1;
    p1 = head->next;
    int k,j;
    struct message near;
    struct user    name[10];
    memset(&name,0,sizeof(struct user));
    memset(&near,0,LEN1);
    near = chat;
    char  near_buf[BUFSIZE];
    near.type = 'z';
    while(p1)
    {
        if(!(strcmp((p1->user).username,near.from)))
        {
            for(k = 0;k < 10;k++)
                memcpy(name[k].username,(p1->friend)[k].username,10);
            for(k = 0;k < 10;k++)
            {
                for(j = 0;j < 30;j++)
                {
                    if((!strcmp(name[k].username,connect_info[j].name)) && (connect_info[j].fd != -1))
                        {
                            memcpy(near.name[k].username,connect_info[j].name,10);
                        }
                }
            }
            
        }
        p1 = p1->next;
    }
    memset(near_buf,0,BUFSIZE);
    memcpy(near_buf,&near,LEN1);
    if(send(connect_info[i].fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        pthread_exit(0);
    }
}
void out_line(struct message chat,int i)
{
    struct message near;
    memset(&near,0,LEN1);
    near = chat;
    connect_info[i].fd = -1;
    strcpy(connect_info[i].name," ");
    save();
    printf("[用户]%s下线了 \t%s\n",near.from,my_time());
    pthread_exit(0);
}
void refuse_add(struct message chat,int i)
{
    int k ;
    char  near_buf[BUFSIZE];
    struct message near;
    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near = chat;
    for(k = 0;k < 30;k++)
    {
        if(!strcmp(connect_info[k].name,near.from))
        {
            memcpy(near_buf,&near,LEN1);
            send(connect_info[k].fd,near_buf,BUFSIZE,0);
        }
    }
    
}
void accept_add_friend(struct message chat,int i)
{
    int k;
    char  near_buf[BUFSIZE];
    struct message near;
    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near = chat;
    for(k = 0;k < 30;k++)
    {
        if(!strcmp(connect_info[k].name,near.to))
        {
            memcpy(near_buf,&near,LEN1);
            send(connect_info[k].fd,near_buf,BUFSIZE,0);
        }
    }
    if(k == 29)
    {
        near.type = 'm';
       // memcpy(near.news,"未找到此用户或者他不在线"，30);
        memcpy(near_buf,&near,LEN1);
        send(connect_info[i].fd,near_buf,BUFSIZE,0);
    }

}
void add_friend(struct message chat,int i)
{
    int k;
    struct users *p1;
    p1 = head->next;
    char  near_buf[BUFSIZE];
    struct message near;
    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near = chat;
    printf("chat.from : %s\n",near.from);
    printf("chat.to  : %s\n",near.to);
    while(p1)
    {
        if(!(strcmp(near.from,(p1->user).username)))
        {
            memcpy(((p1->friend)[p1->friend_num].username),near.to,10);
            p1->friend_num++;
        }
        if(!(strcmp(near.to,(p1->user).username)))
        {
            memcpy(((p1->friend)[p1->friend_num].username),near.from,10);
            p1->friend_num++;  //增加完之后让他的朋友数加1
        }
        p1 = p1->next;
        save();
    }
    memcpy(near_buf,&near,LEN1);
    for(k = 0;k < 30;k++)
    {
        if(!strcmp(connect_info[k].name,near.from))
        {
            if(send(connect_info[k].fd,near_buf,BUFSIZE,0) != BUFSIZE)
            {
                my_err("send",__LINE__);
                pthread_exit(0);
            }
        }
    }
}
void view_friend(struct message chat,int i)
{
    struct  users *p1;
    p1 = head->next;
    int k;
    struct message near;
    memset(&near,0,LEN1);
    near = chat;
    char  near_buf[BUFSIZE];
    near.type = 'v';
    while(p1)
    {
        if(!(strcmp((p1->user).username,near.from)))
        {
            for(k = 0;k < 10;k++)
                memcpy(near.name[k].username,(p1->friend)[k].username,10);
        }
        p1 = p1->next;
    }
    memset(near_buf,0,BUFSIZE);
    memcpy(near_buf,&near,LEN1);
    if(send(connect_info[i].fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        pthread_exit(0);
    }
}
void near_friend(struct message chat,int i)
{
    int  k;
    struct message near;
    char   near_buf[BUFSIZE];
    memset(&near,0,LEN1);
    near = chat;
    for(k = 0;k < 30;k++)
    {
       // printf("connect_info[k].fd :%d,connect_info[k].name:%s\n",connect_info[k].fd,connect_info[k].name);
        if(connect_info[k].fd != -1)
            memcpy(near.name[k].username,connect_info[k].name,10);
    }
    
    memset(near_buf,0,BUFSIZE);
    memcpy(near_buf,&near,LEN1);
    if(send(connect_info[i].fd,near_buf,BUFSIZE,0) != BUFSIZE)
    {
        my_err("send",__LINE__);
        pthread_exit(0);
    }
}
void quit(struct regi_sign account,int i)
{
    struct regi_sign quit;
    quit = account;
    connect_info[i].fd = -1;
    strcpy(connect_info[i].name," ");
    save();
    printf("客户端退出 \t%s\n",my_time());
    pthread_exit(0);
}
struct users* sign_in(struct regi_sign account,int i)
{
    int  k,m = 0;
    struct regi_sign  sign;
    struct users  *p1;
    p1 = head->next;
    sign = account;
    printf("111111111111\n");
    for(k = 0;k < 30; k++)
    {
        if(!strcmp(connect_info[k].name,sign.username))
            m = m+1;
    }
    printf("m:%d\n",m);
    if(m >= 1)
    {
        send(connect_info[i].fd,"w",BUFSIZE,0);
        return NULL;
    }
    if(m == 0)
    {
        while(p1)
        {
            if((!strcmp((p1->user).username,sign.username)) && (!strcmp(p1->password,sign.password)))
            {
                if((send(connect_info[i].fd,"y",BUFSIZE,0)) != BUFSIZE)
                {
                    my_err("login send",__LINE__);
                }
                printf("[用户]%s上线了 \t%s\n",sign.username,my_time());
                memcpy(connect_info[i].name,sign.username,10);
                memcpy(leave_message[y++].name,sign.username,10);
            
                return p1;
            }
            p1 = p1->next; //判断未成功，让p1指向下一个
        }
        if(p1 == NULL)
        {
            send(connect_info[i].fd,"n",BUFSIZE,0);
            return NULL;
        }
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
    memset(p2,0,LEN);         //对刚malloc的结构体进行清空
    strcpy((p2->user).username,apply.username);
    strcpy(p2->password,apply.password);
    p1->next = p2;
    p2->next = NULL;
    save();
    send(connect_info[i].fd,"y",1024,0);
    memcpy(connect_info[i].name,apply.username,10);
    memcpy(leave_message[y++].name,apply.username,10);
    printf("[用户]%s注册成功 \t%s\n",apply.username,my_time());
    return (p2);
}
void *sign_quit()
{
    char choice[20];
    do
    {
        printf("-------退出服务器(请输入quit)--------\n");
        scanf("%s",choice);
    }while(strcmp("quit",choice));
    if(!strcmp("quit",choice))
    {
        save();
        exit(0);
    }
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
    while(1)
    {
        struct    regi_sign account;
        memset(&account,0,sizeof(struct regi_sign));
        memset(recv_buf,0,sizeof(recv_buf));
        if(recv(connect_info[i].fd,recv_buf,sizeof(recv_buf),0) != BUFSIZE)
        {
            my_err("recv",__LINE__);
            pthread_exit(0);
        }
        pthread_mutex_lock(&mutex);
        memcpy(&account,recv_buf,sizeof(recv_buf));
        head = read_input(); //保存头结点
        if(account.flag == 'r')  //注册
        {
            p = apply_account(account,i);
            pthread_mutex_unlock(&mutex);
            leave_message_func(account,i);
            if(p != NULL)
                break;
        }
        if(account.flag == 'l')  //登录
        {
            p = sign_in(account,i);
            pthread_mutex_unlock(&mutex);
            leave_message_func(account,i);
            if(p != NULL)
                break;
        }
        if(account.flag == 'q')  //退出
        {
            pthread_mutex_unlock(&mutex);
            quit(account,i);
        }
    }
    while(1)
    {
        struct    message  chat;
        char      recv_buf[BUFSIZE];
        int       ret;
        memset(&chat,0,LEN1);
        memset(recv_buf,0,BUFSIZE);
        printf("wait\n");
        if((ret = recv(connect_info[i].fd,recv_buf,BUFSIZE,0))!= BUFSIZE)
        {
            printf("ret : %d\n",ret);
            printf("%s\n",recv_buf);
            my_err("recv",__LINE__);
            pthread_exit(0);
        }
        memcpy(&chat,recv_buf,BUFSIZE);
        printf("this is chat.type:%c\n",chat.type);
        switch(chat.type)
        {
            case 'f':   //查看附近的人
            {
                near_friend(chat,i);
                break;
            }
            case 'v':   //查看所有好友
            {
                view_friend(chat,i);
                break;
            }
            case 't':   //添加好友
            {
                add_friend(chat,i);
                break;
            }
            case 'g':   //下线
            {
                out_line(chat,i);
                break;
            }
            case 'z':   //在线好友
            {
                view_online_friend(chat,i);
                break;
            }
            case 's':   //删除好友
            {
                del_friend(chat,i);   
                break;
            }
            case 'o':   //私聊
            {
                pri_chat(chat,i);
                break;
            }
            case 'j':   //建群
            {
                build_group(chat,i);
                break;
            }
            case 'c':   //查看群信息
            {
                view_group(chat,i);
                break;
            }
            case 'b':   //群聊
            {
                public_chat(chat,i);
                break;
            }
            case 'y':   //接受添加好友
            {
                accept_add_friend(chat,i);
                break;
            }
            case 'e':   //拒绝添加
            {
                refuse_add(chat,i);
                break;
            }
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
    int                          ret,i;
    int                          optval;
    time_t                       now;
    pthread_t                    thid,quit_thid;
    
    pthread_mutex_init(&mutex,NULL);
    if((sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        my_err("socket",__LINE__);
    }
    optval = 1;
    if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int)) < 0)
    {
        my_err("setsockopt",__LINE__);
    }

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    pthread_create(&quit_thid,NULL,(void*)sign_quit,NULL);
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
        strcpy(leave_message[i].name," ");
        leave_message[i].number = 0;
    }
    conn_len = sizeof(struct sockaddr_in);
    while(1)
    {
        if((conn_fd= accept(sock_fd,(struct sockaddr *)&conn_addr,&conn_len)) < 0)
        {
            my_err("accept",__LINE__);
        }
        for(i = 0;i < 30;i++)
        {
            if(connect_info[i].fd == -1)
                break;
        }
        connect_info[i].fd = conn_fd;
        time(&now);
        printf("有新的连接：ip:%s\t%s\n",inet_ntoa(conn_addr.sin_addr),ctime(&now));
        int f = i;
        pthread_create(&thid,NULL,client_t,(void*)&f);
    }
     return 0;
}
