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

#define MAX_QUEUE_LENGTH 1024

int main(int argc,char *argv[])
{
    int                          sock_fd,conn_fd;
    struct     sockaddr_in       serv_addr,conn_addr;
    socklen_t                    conn_len;
    pid_t                        pid1;
    char                         recv_buf[128];
    int                          ret;

    //创建套接字
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    /*参数分别为(使用IPV4 tcp/ip协议,使用tcp流套接字,
    通过前两个参数来确定使用的协议类型，默认为零)*/

    //mZ服务器端的套接字进行初始化
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    //memset函数将serv_addr 用0进行初始化
    serv_addr.sin_family = AF_INET;
    /*设置地址类型：对于sin_family,表示用tcp/ip协议编程，
     所以此值只能为AF_INEF*/
    serv_addr.sin_port = htons(4507);
    //设置端口号为4507
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //关于ANADDR——ANY这个宏的解释在下面有,也可以向下面一样设定指定IP
    //inet_pton(AF_INET,"XXX.XXX.XXX.XXX",&serv_addr.sin_addr);
    //当我只有一块网卡，一个IP，所以也只能设置为自己的ip了


    //绑定套接字
    bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in));
/*绑定套接字的过程是将我前面创建的套接字与初始化的端口绑定起来， 因为socket只是创建了一个套接字，这个套接字将在哪个端口上工作， 并没有被指定，作为服务器，它的IP和端口一般是固定的，因此我们需要将刚才
初始化的端口和套接字绑定到一起，这时套接字已经完成服务器的ip和端口这一半*/

    //将套接字转化成监听套接字
    listen(sock_fd,MAX_QUEUE_LENGTH);
    //第二个参数比较重要，它是我们服务器已完成套接字队列的长度，accept函数每次
    //会从已完成的套接字队列中拿走一个即conn_fd，也就是监听套接字，注意：内核会维护两个队列，
    //一个是已完成的套接字队列，一个是未完成的套接字队列,一个套接字完成的过程是在三次握手的
    //过程中完成的，是由tcp/ip协议栈完成的


    //接受客户端的请求
    conn_len = sizeof(struct sockaddr_in);
    while(1)
    {
        conn_fd = accept(sock_fd,(struct sockaddr *)&conn_addr,&conn_len);
        //服务器会将客户端的连接消息先放在未完成的队列中，三次握手之后，就到已完成队列中
        //conn_fd就是accept返回的叫作连接套接字，它的信息有服务器的端口和IP以及客户的端口和IP，
        //监听套接字继续监听，对于新的处理，系统会重新开一个线程处理，自己也可以用进程
        printf("accept a new connection,ip:%s\n",inet_ntoa(conn_addr.sin_addr));
        pid1 = fork();
        if(pid1 == 0)
            ret = recv(conn_fd,recv_buf,sizeof(recv_buf),0);
        recv_buf[ret-1] = '\0';
    }
    printf("%s\n",recv_buf);
}
