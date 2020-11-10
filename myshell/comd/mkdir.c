#include <stdio.h>
#include <stdlib.h>

void mkdir1(int argc, char *argv[])
{
    //printf("mkdir irectory success.\n");

	if(strcmp(argv[1], "-p")==0)
	{
        	int i,len;
        	char str[512];
        	for(int j=2; j<argc; j++)
        	{
            		strncpy(str, argv[j], 512);
            		len=strlen(str);
            		for( i=0; i<len; i++ )
           		{
               			if( str[i]=='/' )
                		{
                    		str[i] = '\0';
                    		if( access(str,0)!=0 )
                    		{
                        		mkdir( str, 0777 );
                    		}
                    		str[i]='/';
                		}
            		}
            		if( len>0 && access(str,0)!=0 )
            		{
                		mk(str);
            		}
        	}

	}
	else
    	{
        	for(int j=1; j<argc; j++)
        	    	/*if(mkdir(argv[j], 0777)!=0)
			{
				printf("%s:目录创建失败\n", argv[j]);
			}
			else
			{
				printf("%s:目录创建成功\n", argv[j])
			}*/
			mk(argv[j]);
    	}
}

void mk(char *dirname)
{
	if(mkdir(dirname, 0777)!=0)
		printf("%s:目录创建失败\n", dirname);
	else
		printf("%s:目录创建成功\n", dirname);

}

int main(int argc, char *argv[])
{
	mkdir1(argc, argv);
	return 0;
}
