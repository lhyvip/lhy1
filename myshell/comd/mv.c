#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
void main(int argc, char * argv[] )
{
	FILE * source = fopen(argv[1],"r");
	if(source==NULL)
	{
		perror ("file no exsit!\n");
		return ;
	}
	else
	{
		struct stat * statbuf=(struct stat *)malloc(sizeof(statbuf));
		stat(argv[2], statbuf);
               
		if(S_ISDIR (statbuf->st_mode)) 
		{
				int i,j,k=0;
				
				for(i=strlen(argv[1])-1;i>=0;i--)
				{
					if(argv[1][i]=='/') 
					{
						break; 
					}
				}
				char source_file_name[128]={};
				for(j=i+1;j<strlen(argv[1]);j++)
				{	
					source_file_name[k++]=argv[1][j];
				}
				
				if(argv[2][strlen(argv[2])-1]!='/') 
				{
					strcat (argv[2],"/");
				}
				
				FILE * target = fopen(strcat (argv[2],source_file_name),"w+");
				while(!feof(source))
				{	
					char  buf[10]={};
					fread (buf,1,10,source); 
					fwrite (buf,1,10, target);
				}	
				
		}
		else
		{
			FILE * target = fopen(argv[2],"w+");
			while(!feof(source))
			{
				char  buf[10]={};
				fread (buf,1,10,source);
				fwrite (buf,1,10, target);
			}	
				fclose(target);
		}
		
	}
	remove(argv[1]);
	fclose(source);

}



