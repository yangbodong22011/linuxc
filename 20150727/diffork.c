/*************************************************************************
	> File Name: difffork.c
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Jul 2015 01:49:47 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

    int globVar = 5;
    int main(int argc,char *argv[])
    {
        int var = 1,i;
        pid_t pid;

        printf("fork is different with vfork\n");

      pid = fork();
//        pid = vfork();
        printf("%d---------\n",pid);
        switch(pid)
        {
            case 0:
            i = 2;
            while(i-- > 0)
            {
                printf("--------Child process is running\n");
                globVar++;
                var++;
                printf("--------c  sleep\n");
                sleep(3);
            }
            printf("--------Clild's globVar = %d,var = %d\n",globVar,var);
            break;
            case -1:
                perror("Process creat faild\n");
                exit(0);
            default:
                i = 3;
                while(i-- > 0)
                {
                printf("Parent process is running\n");
                globVar++;
                var++;
                printf("p sleep\n");    
                sleep(3);

                }
                printf("Parent's globVar = %d,var = %d\n",globVar,var);
                exit(0);
        }
        exit(0);
    }
