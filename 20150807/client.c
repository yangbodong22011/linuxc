/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Fri 07 Aug 2015 09:53:41 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(int argc,char *argv)
{
    struct sockaddr_in      serv_addr;
    int                     conn_fd;

    //同样我们需要在客户端也建立套接字
    conn_fd = socket(AF_INET,SOCK_STREAM,0);

    //初始化与服务器端匹配的信息
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4507);
    inet_aton("192.168.20.144",&serv_addr.sin_addr);
    
    //用于向服务器端发送连接请求,服务器的IP地址和端口号由参数serv_addr指定
    connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in));
    close(conn_fd);
}
