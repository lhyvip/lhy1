#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<pwd.h>
#include<string.h>
#define MAX_CMD 255
#define MAX_DIR_NAME 255


//help�����ĵ�
int helps(char *inputs[],int i)
{

    if(i==1)
    {
        printf("These shell commands are defined internally.  Type 'help' to see this list.\n");
        printf("Type 'help name' to find out more about the function 'name'\n");
        return 1;
    }
    else if (strcmp(inputs[1],"pwd")==0)
    {
        printf("pwd:��ӡ��ǰ����·��\n");
        return 1;
    }
    else if(strcmp(inputs[1],"cd")==0)
    {
        printf("cd:�����л���ǰĿ¼\n");
        return 1;
    }
    else if(strcmp(inputs[1],"exit")==0)
    {
        printf("exit:�˳�shell\n");
        return 1;
    }
    else if(strcmp(inputs[1],"echo")==0)
    {
        printf("echo:��ʾ������\n");
        return 1;
    }
    return 0;
}



//��дһ�����������ר��д����ָ��
//����ֵΪ1ʱΪ�ɹ�ִ������ָ��
//����0ʱΪִ��ʧ�ܲ�����������
int build_out_command(char *inputs[],int i)
{
    char path[]="./comd/";
    char buffer[10];
    bzero(buffer,10);
    int fd[2];
    if(pipe(fd)==-1)
    {
        return 0;
    }
    int rc=fork();
    if(rc<0)
    {
        return 0;
    }
    else if(rc==0)
    {
//�ص���
        close(fd[0]);
        if(execv(strcat(path,inputs[0]),inputs)<0)    //strcat(path,inputs[0])
        {
            write(fd[1],"false",10);
        }
        else
        {
            write(fd[1],"true",10);
        }
        close(fd[1]);
        exit(0);
//�����ӽ���
    }
    else if(rc>0)
    {
        close(fd[1]);
        wait(NULL);
        read(fd[0],buffer,10);
        if(strcmp(buffer,"false")==0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}




//��дһ�����������ר��д����ָ��
//����ֵΪ1ʱΪ�ɹ�ִ������ָ��
//����0ʱΪ������������
int build_in_command(char cmdstring[],char *inputs[],int i)
{
//1.ʵ��exit�˳�
//printf("inputs[0]=%s",inputs[0]);
    if(strcmp(inputs[0],"exit")==0)
    {
        printf("Bye.\n");
        exit(0);
    }
//2.ʵ��pwd����Ŀ¼
    else if(strcmp(inputs[0],"pwd")==0)
    {
        char path[MAX_DIR_NAME];
        memset(path,0,MAX_DIR_NAME);
        printf("%s\n",getcwd(path,MAX_DIR_NAME));
        return 1;
    }
//3.ʵ��cd�ı�Ŀ¼
    else if(strcmp(inputs[0],"cd")==0)
    {
        if(chdir(inputs[1])==0)
        {
            return 1;
        }

    }
//4.echo��ʾ������
    else if(strcmp(inputs[0],"echo")==0)
    {
        char *buf1=cmdstring;
        while(*buf1==' ')
        {
            buf1++;
        }
        while(*buf1!=' ')
        {
            buf1++;
        }
        while(*buf1==' ')
        {
            buf1++;
        }

        printf("%s\n",buf1);
        return 1;

    }
//help�����ĵ�
    else if(strcmp(inputs[0],"help")==0)
    {
        int i1= helps(inputs,i);
        return i1;
    }



    return 0;
}


//������ʾ��
void attention()
{
    struct passwd *pwd1;
    pwd1=getpwuid(getuid());
    char path[MAX_DIR_NAME];
    memset(path,0,MAX_DIR_NAME);
    getcwd(path,MAX_DIR_NAME);
    int i=0;
    int len=strlen(path);
    char *p=path;

    int i1=len;
    for(i1; i1>=0; i1--)
    {
        if(path[i1]=='/')
        {
            path[i1]='\0';
            break;
        }
    }
    for(i; i<=i1; i++)
    {
        p++;
    }
    printf("<%s@localhost :%s#> ",pwd1->pw_name,p);
}





//����ָ�� ����ls -l  ������ָ���Ϊls��-l�����ַ����ֱ�洢��inputs�ַ���������
//����ֵ�Ǵ洢�ĸ������һλΪNULLһ��i+1��
int parsecommand(char buf[],char *inputs[])
{
    bzero(inputs,MAX_CMD);
    int in=0;
    char *p=buf;
    int i=0;
    while(*p==' ')
    {
        p++;
        i++;
    }
    inputs[in]=p;
    in++;
    int i1=i;
    int len=strlen(buf);
    for(i; i<=len; i++)
    {
        if(buf[i]==' ')
        {
            buf[i]='\0';

        }
    }
    i1++;
    p++;
    for(i1; i1<=len; i1++,p++)
    {
        if(buf[i1]!='\0')
        {
            if(buf[i1-1]=='\0')
            {
                inputs[in]=p;
                in++;
            }

        }
    }
    inputs[in]=NULL;

    return in;

//һ����in+1���ַ���,���һλNULL

}


//����ִ�к�����Ϊ�ڲ�������ⲿ����
//�ڲ�����ֱ�ӵ��ú���ִ�оͺ�
//�ⲿ�����Լ���д����һ���ļ���ͨ��gcc����
//�ڸú�����ͨ��exec���庯������ִ��
int  eval(char cmdstring[])
{
    char *inputs[MAX_CMD];
    char buf[MAX_CMD];
    strcpy(buf,cmdstring);
    int i=parsecommand(buf,inputs);
//����ʵ��һЩ����
//�����������ú���

    int returnin=build_in_command(cmdstring,inputs,i);
    if(returnin==0)
    {
        int rtout=build_out_command(inputs,i);
        if(rtout==0)
        {
            printf("%s: not find command\n",cmdstring);
            return 0;
        }
    }
    return 1;
}




//main����ͨ��ѭ�����Ͻ����û�����
//����eval���ܺ���ʵ��
int main(int argc,char *argv[])
{
    char cmdstring[MAX_CMD];
    bzero(cmdstring,MAX_CMD);
    while(1)
    {
        attention();
        fflush(stdout);
//��ȡ������
        fgets(cmdstring,MAX_CMD,stdin);
//���û��������¿�ʼ
        if(cmdstring[0]=='\n')
        {
            continue;
        }
//ȥ��ĩβ�ġ�\n����Ϊ��\0��
        int i=0;
        for(i; cmdstring[i]!='\n'; i++)
        {
            ;
        }
        cmdstring[i]='\0';

        int p=eval(cmdstring);
    }
    return 0;

}

