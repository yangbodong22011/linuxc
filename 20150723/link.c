/*************************************************************************
	> File Name: link.c
	> Author: 
	> Mail: 
	> Created Time: Thu 23 Jul 2015 11:41:41 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#define LEN sizeof(struct student)


struct student
{
    int Id;
    char name[20];
    struct student *next;
};

typedef struct student * head;

head Input(head h);
head Delete();
int Ruin(head h);
void Print();
head Find();
void biaotou();

void biaotou()
{
    printf("\n              信息表\n");
    printf("学生编号\t学生姓名\n");
}
head Find(head h)
{
    int Id;
    head p;

    printf("请输入学生编号\n");
    scanf("%d",&Id);        
    for(p = h ;p->next;p = p->next)
    {    
        if(p->next->Id == Id)
        {
            biaotou();
            printf("%d\t\t%s\n",p->next->Id,p->next->name);
            break;
        }
    }
        if(p->next == NULL)
        {
            printf("没有此学号\n");
        }
        

    return(p);
}
void Print(head h)
{
    head p;
    
    printf("\n              信息表\n");
    printf("学生编号\t学生姓名\n");
    for(p = h->next; p; p = p->next)
    printf("%d\t\t%s\n",p->Id,p->name);


}
int  Ruin(head h)
{
    head p = NULL;
    int choice;

    printf("确定销毁整个链表？(1:销毁，2：不销毁)\n");
    scanf("%d",&choice);
    if(choice == 1)
    {
    for(p;h->next;)
    {
        p = h->next;
        h->next = p->next;
        free(p);
    }
    free(h);
        printf("销毁成功\n");
    }
    if(choice == 2)
    {
        return 0;
    }
}
head Delete(head t)
{
    head p;
    
    p = t->next;
    t->next = p->next;
    free(p);
    printf("删除成功\n");

}

void Add(head t)
{
    head p ;

    p = (head) malloc (LEN);
    printf("请输入要插入学生的信息\n");
    printf("请输入学生Id\n");
    scanf("%d",&p->Id);
    printf("请输入学生姓名\n");
    scanf("%s",p->name);

    p->next = t->next;
    t->next = p;

    
}
head Input(head h)
{
    char choice;
    head p;
    head q;
    h = (head) malloc(LEN);
    q = h;
    do
    {
        p = (head) malloc(LEN);
        printf("请输入学生Id\n");
        scanf("%d",&p->Id);
        printf("请输入学生姓名\n");
        scanf("%s",p->name);
        p->next = NULL;
        h->next = p;
        h = p;
        printf("Contine?(Y/N)\n");
        getchar();
        scanf("%c",&choice);
    }while(choice == 'Y'||choice == 'y' );
    
    return(q);

}



int main(int argc,char *agrv[])
{
    head  h = NULL;
    head  t = NULL;     //查找到的学生信息id所指的指针
    int choice;

    printf("学生信息\n");
    do
    {
        printf("1:输入信息\n");
        printf("2:增加信息\n");
        printf("3:删除信息\n");
        printf("4:销毁信息\n");
        printf("5:显示信息\n");
        printf("6:查找信息\n");
        printf("0:退出程序\n");
        printf("请输入：");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:h = Input(h);
            break;
            case 2:
            printf("请先确定要插入的位置学号（前插入）是否存在\n");
            printf("注意：若不存在，默认插入到最后\n");
            t = Find(h);
            Add(t);
            break;
            case 3:
            printf("请先确定要删除的学号是否存在\n");
            t = Find(h);
            Delete(t);
            break;
            case 4:Ruin(h);
            break;
            case 5:Print(h);
            break;
            case 6:Find(h);
            break;
        }
    }while(choice != 0);
}
