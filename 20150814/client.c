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
#define BUFSIZE 1024
#define LEN1      sizeof(struct message)
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
    int          group_num;
    struct user  friend[10];
    struct group group[10];
    struct users *next;
};



struct message 
{
    char         type;
    char         from[10];
    char         to[10];
    char         time[10];
    char         changefriname[10];
    struct       user name[30];
    struct       group group;
    char         news[500];
};
int              friend_num;  //好友数，全局变量
char             myname[10];        //存自己用户的名字
struct  user     friend[10];  
struct  user     member[10];
struct  group    group[3];

char * my_time()
{
    time_t  now;
    time(&now);
    return (ctime(&now));
}

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
    buf[i++] = '\0';
    buf[i++] = '\n';

    return 0;
}
int sign_in();
int apply_account();
void quit();
void * sign_func();
void login_menu();
int near_friend();    //附近的人
int admin_friend();   //好友管理
int view_friend();    //查看所有好友
int view_online_friend();
int add_friend();
void out_line();
void del_friend();
int chat_with();
int pri_chat();
int build_group();
int view_group();
int public_chat();
void view_chat_record();

void view_chat_record()
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
    while(!feof(fp))
    {
        fread(&near,LEN1,1,fp);
        printf("%s %s %s\n ",near.time,near.from,near.news);
    }
    fclose(fp);
}
int public_chat()
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'b';
    memcpy(near.from,myname,10);
    printf("请输入群名\n");
    my_input(near.to,10);
    memcpy(near.time,my_time(),10);
    system("clear");
    printf("-------你正在群 %s 里聊天，谨防诈骗！(如果想退出，输入quit)-------\n",near.to);
    while(1)
    {
        printf("%s",my_time());
        printf("%s说：",near.from);
        memset(near.news,0,100);
        my_input(near.news,100);
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
int view_group()
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
int build_group()
{
    int    i,k;
    int    number;
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'j';
    memcpy(near.from,myname,10);
    memcpy(near.time,my_time(),10);
    printf("请输入要建立的群名称:\n");
    my_input(near.group.group_name,10);
    printf("请输入群成员个数(0-9):");
    scanf("%d",&number);
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
int pri_chat()
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 'o';
    memcpy(near.from,myname,10);
    printf("请输入您私聊的好友姓名\n");
    my_input(near.to,10);
    memcpy(near.time,my_time(),10);
    system("clear");
    printf("-------你正在与 %s 聊天(如果想退出，输入quit)-------\n",near.to);
    while(1)
    {
        printf("%s",my_time());
        printf("%s说：",near.from);
        memset(near.news,0,100);
        my_input(near.news,100);
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
    do
    {
        printf("1:我要私聊\n");
        printf("2:查看我的群\n");
        printf("3:我要建群\n");
        printf("4:我要群聊\n");
        printf("0:返回上级菜单\n");
        while(1)
        {  
            printf("请选择(0 ~ 4)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2"))||(!strcmp(choice,"3")) || (!strcmp(choice,"4")))
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
                public_chat();
                break;
            }
            case 0:
            {
                return 0;
            }
        }
    }while(strcmp(choice,"-1"));

}
void del_friend()
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
void out_line()
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
    close(conn_fd);
    exit(0);

}
int add_friend()
{
    char   near_buf[BUFSIZE];
    struct message near;

    memset(&near,0,LEN1);
    memset(near_buf,0,BUFSIZE);
    near.type = 't';
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
int view_online_friend()
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


int view_friend()
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

int admin_friend()
{
    char   choice[20];
    do
    {
        printf("1:查看所有好友\n");
        printf("2:查看在线好友\n");
        printf("3:添加好友\n");
        printf("4:删除好友\n");
        printf("0:返回上级菜单\n");
        while(1)
        {  
            printf("请选择(0 ~ 4)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2"))||(!strcmp(choice,"3")) || (!strcmp(choice,"4")))
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
            case 0:
            {
                return 0;
            }
        }

    }while(strcmp(choice,"-1"));
}
int near_friend()
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
void login_menu()
{
    char     choice[20];
    struct   message near;
    do
    {
       // printf("\t\t %s\n",myname);
        printf("1:附近的人\n");
        printf("2:好友管理\n");
        printf("3:我要聊天\n");
        printf("4:查看聊天记录\n");
        printf("0:我要下线\n");
        while(1)
        {  
            printf("请选择(0 ~ 4)：");
            setbuf(stdin,NULL);
            scanf("%s",choice);
            if((!strcmp(choice,"0")) || (!strcmp(choice,"1")) ||(!strcmp(choice,"2"))||(!strcmp(choice,"3")) || (!strcmp(choice,"4")))
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
        //printf("wait\n");
        //printf("conn_fd:%d\n",conn_fd);
        sleep(1);
        if(recv(conn_fd,recv_buf,BUFSIZE,0) != BUFSIZE)
        {
            my_err("服务器退出",__LINE__);
            pthread_exit(0);
        }
        memcpy(&chat,recv_buf,BUFSIZE);
        switch(chat.type)
        {
            case 'f':
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
            case 'v':
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
            case 't':
            {
                printf("\n");
                printf("[用户]%s 已经成为您的好友了\n",chat.to);
                break;
            }
            case 'z':
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
            case 's':
            {
                printf("\n");
                printf("[用户]%s 已经被您删除\n",chat.to);
                break;
            }
            case 'o':
            {
              //  char name[10];
              //  memcpy(name,chat.from,10);
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
            case 'j':
            {
                printf("\n");
                printf("%s",my_time());
                printf("[系统提示] 群‘%s’ 已经建立成功\n",chat.group.group_name);
                break;
            }
            case 'c':
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
            case 'b':
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
                printf("%s",my_time());
                printf("[群：%s]%s 说：%s\n",chat.to,chat.from,chat.news);
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
        printf("请输入密码：");
        my_input(apply.password,10);
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
            printf("用户名已被使用，重新注册\n");
            return 0;
        }
        if(strcmp(apply_buf,"y") == 0)
        {
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
        my_input(apply.password,10);
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
            printf("用户名或密码有错误\n");
            return 0;
        }
        if(strcmp(apply_buf,"y") == 0)
        {
            printf("登录成功\n");
            memset(myname,0,sizeof(struct user));
            memcpy(myname,apply.username,sizeof(apply.username));
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
