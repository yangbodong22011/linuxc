/*************************************************************************
	> File Name: my_opendir.c
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Jul 2015 08:33:58 AM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    DIR *dir;
    struct dirent *ptr;

    if((dir = opendir("test")) == NULL)
    {
        perror("opendir error");
        exit(1);
    }
    while((ptr = readdir(dir)) != NULL)
    {
        printf("%s\n",ptr->d_name);
        
    }
   // closedir(dir);

}
