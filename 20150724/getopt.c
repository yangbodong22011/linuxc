/*************************************************************************
	> File Name: getopt.c
	> Author: 
	> Mail: 
	> Created Time: Fri 24 Jul 2015 08:37:30 AM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<getopt.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<dirent.h>
#include<string.h>

#define MAXROWLEN  80;
int  g_leave_len = MAXROWLEN;
int  g_maxlen;
int  opterr = 0;

void show_name(char *path);
void display_single(char *path);

void display_single(char *path)
{
    int i;
    int len;
    if(g_leave_len < g_maxlen)
    {
        printf("\n");
        g_leave_len = MAXROWLEN;
    }
    len = strlen(path);
    len = g_maxlen - len;
    printf("%-s",path);
    for(i = 0; i < len; i++)
    {
        printf(" ");
    }
    printf(" ");
    g_leave_len -= (g_maxlen + 2);

    
}

//void display__attribute(struct );

void show_name(char *path)
{
    DIR *dir;               
    struct dirent *ptr;                         
    if((dir = opendir(path)) == NULL)
    {
        perror("opendir error");
        exit(1);
    }
    while((ptr = readdir(dir)) != NULL)
    {
        if(g_maxlen < strlen(ptr->d_name))
        g_maxlen = strlen(ptr->d_name);
    }
    closedir(dir);
    if((dir = opendir(path)) == NULL)
    {
        perror("opendir error");
        exit(1);
    }
    while((ptr = readdir(dir)) != NULL)
    {
        display_single(ptr->d_name);
    }
    printf("\n");
    closedir(dir);
 
}

int main(int argc,char *argv[])
{
    int opt;
 //   struct stat buf;

    while((opt = getopt(argc,argv,"a:l:")) != -1)
    {
        printf("%d\n",opt);
        switch(opt)
        {
            case 'a':
                show_name(optarg);
                break;
            case 63:
                strcpy(optarg,"./");
                show_name(optarg);
                break;
            case 'l':

          //  display__attribute()
            break;
            default:
            fprintf(stderr,"Usage: %s [-t nsecs] [-n] name\n",argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    /*printf("flag=%d;tfind=%d;optind=%d\n",flags,tfind,optind);
    if(optind >= argc)
    {
        fprintf(stderr,"Expected argument after options\n");
        exit(EXIT_FAILURE);
    }
    printf("name argument = %s\n",argv[optind]);*/
 

































/*    int opt;
    while((opt = getopt(argc,argv,"nb:o::t")) != -1)
    {
        printf("opt = %c,optarg =%s,optind = %d,argv[%d] = %s\n",opt 
              ,optarg,optind,optind,argv[optind]);
    }
*/
    return 0 ;
}
