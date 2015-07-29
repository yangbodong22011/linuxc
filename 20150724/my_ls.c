/*************************************************************************
	> File Name: my_ls.c
	> Author: 
	> Mail: 
	> Created Time: Fri 24 Jul 2015 08:20:19 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<linux/limits.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>
#include<time.h>

#define PARAM_NONE  0                     //无参数
#define PARAM_A     1                     // -a 显示所有文件
#define PARAM_L     2                     // -l:一行只显示一个文件的详细信息
#define MAXROWLEN   80                    //一行显示的最多字符数

int     g_leave_len = MAXROWLEN;           //一行剩余长度，用于输出对齐
int     g_maxlen;                          //存放某目录下最长文件名的长度

void display_attribute(struct stat buf,char *name)
{
    char buf_time[32];
    struct passwd *psd;                    //从结构体中获取文件所有者用户名
    struct group *grp;                     //从结构体中获取文件所有者组名

    //获取并打印文件类型
    if(S_ISLNK(buf.st_mode))
    {
        printf("l");
    }
    else if(S_ISREG(buf.st_mode))
    {
        printf("-");
    }
    else if(S_ISCHR(buf.st_mode))
    {
        printf("c");
    }
    else if(S_ISBLK(buf.st_mode))
    {
        printf("b");
    }
    else if(S_ISDIR(buf.st_mode))
    {
        printf("d");
    }
    else if(S_ISFIFO(buf.st_mode))
    {
        printf("f");
    }
    else if(S_ISSOCK(buf.st_mode))
    {
        printf("s");
    }
    //获取并打印文件所有者权限
    if(buf.st_mode & S_IRUSR)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWUSR)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXUSR)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    //获取并打印文件与文件所有者同组的用户对文件权限
    if(buf.st_mode & S_IRGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    //获取并打印文件其他用户对文件的操作权限
    if(buf.st_mode & S_IROTH)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWOTH)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXOTH)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }

    printf("    ");
    //根据uid和gid获取文件所有者的用户名与组名
    psd = getpwuid(buf.st_mode);
    grp = getgrgid(buf.st_mode);
    printf("%4d",buf.st_nlink);              //打印文件连接数
    printf("%-8s",psd->pw_name);
    printf("%-8s",grp->gr_name);
    printf("%6d",buf.st_size);               //打印文件大小
    strcpy(buf_time,ctime(&buf.st_mtime));
    buf_time[strlen(buf_time) - 1] = '\0';   //去掉换行符
    printf("  %s",buf_time);                 //打印文件时间信息
}


//在没有使用-l选项时，打印一个文件名，打印时上下对齐    注意！ 它相当于一个输出格式，每输出一个就用一次
void display_single(char *name)
{
    int i,len;
    
    if(g_leave_len < g_maxlen)
    {
        printf("\n");
        g_leave_len = MAXROWLEN;
    }

    len = strlen(name);
    len = g_maxlen - len;
    printf("%-s",name);
    for(i = 0;i < len;i++)
    {
        printf(" ");
    }
    printf(" ");
    g_leave_len -= (g_maxlen + 2);
}

//根据命令行参数和路径显示目标文件
//参数flag ： 命令行参数
//参数pathname ：包含了文件名的路径名

void display(int flag,char *pathname)
{
    int           i,j;
    struct stat   buf;
    char          name[NAME_MAX + 1];

        display_single(name);
    //从路径中解析出文件名
    for(i = 0,j = 0;i < strlen(pathname);i++)
    {
        if(pathname[i] == '/')
        {
            j = 0;
            continue;
        }
        name[j++] = pathname[i]; 
    }
    name[j] = '\0';
    //用lstat 解析链接文件
    if(lstat(pathname,&buf) == -1)
    {
        perror("解析链接文件出错\n");
        exit(1);
    }
    switch(flag)
    {
        case PARAM_NONE:       //没有-l 和 -a 选项
        if(name[0] != '.')
        display_single(name);
        break;
        case PARAM_A:          //-a 选项
        display_single(name);
        break;
        case PARAM_L:           // -l 选项
        if(name[0] != '.')
        {
            display_attribute(buf,name);
            printf(" %-s\n",name);
        }
        break;
        case PARAM_A + PARAM_L :
        display_attribute(buf,name);
        printf(" %-s\n",name);
        break;
        default:
        break;
    }
    
}

void display_dir(int flag_param,char *path)
{
    DIR             *dir;
    struct dirent   *ptr;
    int             count=0;
    char            filenames[600][PATH_MAX + 1],temp[PATH_MAX + 1];

    //获取目录下文件总数和最长的文件
 //   printf("%s\n",path);
 //   printf("%d\n",flag_param);
    if((dir = opendir(path)) == NULL)
    {
        perror("opendir error");
        exit(1);
    }
    while((ptr = readdir(dir)) != NULL) 
    {
        if(g_maxlen < strlen(ptr->d_name))
        g_maxlen = strlen(ptr->d_name);
        count++;
    }
    closedir(dir);
    if(count > 600)                              //目录下文件最多256
    {
       perror("too many");
        exit(1);
    }
    int i,j,len = strlen(path);
    dir = opendir(path);
    //获取该目录下所有的文件名
    for(i = 0;i < count;i++)
    {
        if((ptr = readdir(dir)) == NULL)
        {
            perror("opendir error ");
            exit(1);
        }
    strncpy(filenames[i],path,len);
    filenames[i][len] = '\0';
    strcat(filenames[i],ptr->d_name);
    filenames[i][len+strlen(ptr->d_name)] = '\0';
    }
//使用冒泡法对文件名排序
//    printf("%d\n",count);
    for(i = 0;i < count-1;i++)
        for(j = 0;j < count-1-i;j++)
    {
        if(strcmp(filenames[j],filenames[j+1]) > 0)
        {
            strcpy(temp,filenames[j+1]);
            temp[strlen(filenames[j+1])] = '\0';
            strcpy(filenames[j+1],filenames[j]);
            filenames[j+1][strlen(filenames[j])] = '\0';
            strcpy(filenames[j],temp);
            filenames[j][strlen(temp)] = '\0';

        }
    }
printf("wiwii%d\n",count);

    for(i = 0;i < count;i++)
    {
        display(flag_param,filenames[i]);
    }
    closedir(dir);
    //如果命令行中没有-l参数，打印一个换行符
    if((flag_param & PARAM_L) == 0)
    printf("\n");
}

int main(int argc,char *argv[])
{
    int               i,j,k,num;
    char              path[PATH_MAX+1];
    char              param[32];                        //保存参数
    int               flag_param = PARAM_NONE;          //参数种类
    struct stat       buf;


    //命令行参数的解析
    j = 0;
    num = 0;
//    printf("%d\n",argc);
    for(i = 1;i < argc;i++)
    {
        if(argv[i][0] == '-')
        {
            for(k = 1;k < strlen(argv[i]);j++,k++)
            param[j] = argv[i][k];                      //获取后面的'-'保存到数组中
        num++;
        }
    }
//    printf("%d\n",num);
    for(i = 0;i < j;i++)
    {
        if(param[i] == 'a')
        {
            flag_param |= PARAM_A;
            continue;
        }
        else if(param[i] == 'l')
        {
            flag_param |= PARAM_L;
            continue;
        }
        else
        {
            printf("my_ls : option -%c\n",param[i]);
            exit(1);
        }
    }
    param[j] = '\0';
    if((num + 1 ) == argc)
        {
            strcpy(path,"./");
            path[2] = '\0';
            display_dir(flag_param,path);
            return 0;
        }
    i = 1;
    do
    {
        if(argv[i][0] == '-')
        {
            i++;
            continue;
        }
        else
        {
            strcpy(path,argv[i]);
            //如果目录文件不存在，报错并退出
            if(stat(path,&buf) == -1)
            {
                perror("dir error");
                exit(1);
            }
            if(S_ISDIR(buf.st_mode)) 
            {
                //argv[i] 是一个目录
            //如果最后一个字符不是‘/’,就加‘/’
            if(path[strlen(argv[i]) - 1] != '/')
            {
                path[strlen(argv[i])] = '/';
                path[strlen(argv[i]) + 1] = '\0';
            }
            else
                path[strlen(argv[i])] = '\0';
            display_dir(flag_param,path);
            i++;
            }
            else{
                display(flag_param,path);
                i++;
            }
        }
    }while(i < argc);
    return 0;
}

