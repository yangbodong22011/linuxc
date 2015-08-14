/*************************************************************************
	> File Name: my_recv.h
	> Author: 
	> Mail: 
	> Created Time: Mon 10 Aug 2015 08:16:29 PM CST
 ************************************************************************/
#ifndef _MY_RECV_H
#define _MY_RECV_H
#define BUFSIZE  1024
void my_err(const char *err_string,int line);
int my_recv(int conn_fd,char *data_buf,int len);
#endif
