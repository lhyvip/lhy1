#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

void del_stat(const char* name)
{
	DIR* fl = opendir(name);
	chdir(name);

	for(struct dirent* dir = readdir(fl); NULL!=dir;)
	{
		if(dir->d_type == DT_DIR)
		{
			if(strcmp(dir->d_name,"..")&&strcmp(dir->d_name,"."))
				del_stat(dir->d_name);
		}
		remove(dir->d_name);
		dir = readdir(fl);
	}
	chdir("..");
	remove(name);
}

int main(int argc,char *argv[])
{
    if(strcmp(argv[1], "-rf")==0)
    {
        if(3 != argc)
        {
            printf("User:./rm -rf xxx\n");
            return -1;
        }

        struct stat sta;
        if(0 > stat(argv[2],&sta))
        {
            perror("open");
            return -1;
        }

        if(!S_ISDIR(sta.st_mode))
        {
            //char temp;
            //printf("该文件是一个文件，是否删除(y)、(n):");
            //scanf("%c",&temp);
            //if(temp == 'n')return 0;
            remove(argv[2]);
            return 0;
        }

        del_stat(argv[2]);
    }else
    {
        if(2 != argc)
        {
            printf("User:./rm xxx\n");
            return -1;
        }

        struct stat sta;
        if(0 > stat(argv[1],&sta))
        {
            perror("open");
            return -1;
        }

        if(!S_ISDIR(sta.st_mode))
        {
            //char temp;
            //printf("该文件是一个文件，是否删除(y)、(n):");
            //scanf("%c",&temp);
            //if(temp == 'n')return 0;
            remove(argv[1]);
            return 0;
        }else
        {
            printf("删除失败，该文件是一个目录\n");
            return -1;
        }
    }

}
