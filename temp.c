#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<pwd.h>
#include<string.h>
#include<sys/wait.h>

void getLoginName()
{
	struct passwd* pass;
	pass=getpwuid(getuid());
	printf("\033[40;34m[%s@\033[0m",pass->pw_name);
}

void getHostName()
{
	char name[128];
	gethostname(name,sizeof(name)-1);
	printf("\033[40;34m%s]:\033[0m",name);
}

void getDir()
{
	char pwd[128];
	getcwd(pwd,sizeof(pwd)-1);
	int len=strlen(pwd);
	char* p=pwd+len-1;
	while(*p!='/'&&len--)
	{
		p--;
	}
	p++;
	printf("\033[40;35m %s$\033[0m",p);
}

void filecopy(FILE * ifp, FILE * ofp)
{
    int c;
  while((c = getc(ifp)) != EOF)
        putc(c, ofp);
}//实现赋值文件的功能

int main()
{
	printf("************welcome to ymk shell*************\n");
	while(1)
	{
		getLoginName();
		getHostName();
		getDir();
		fflush(stdout);//读取字符串
		char buffer[1024];
		int s=read(0,buffer,1024);
		if(s>0)//如果有读取到字符
		{
			for(int i=0;i<s;i++)
			{
				if(buffer[i]=='\b'&&i>=1)
				{
					for(int j=i+1;j<s;j++)
					{
						buffer[j-2]==buffer[j];						
					}
					buffer[s-2]=0;
					i-=1;
				}//去掉中间的tab符号
				else if(buffer[i]=='\b'&&i==0)
				{
					int j=0;
					for(j=1;j<s;j++)
					{
						buffer[j-1]=buffer[j];
					}
					buffer[j-1]=0;
					i-=1;
				}//去掉开头的tab符号
				else
				{
					continue;
				}

			}
			buffer[s]=0;			
		}
		else
		{
			continue;
		}

		char * start=buffer;
		int i=1;
		char *myargv[10]={0};
		myargv[0]=start;
		while(*start)
		{
			if(isspace(*start))
			{
				*start=0;
				start++;
				myargv[i++]=start;
			}
			else
			{
				start++;
			}
		}//把字符分个成几个字符串
		myargv[i-1]=NULL;


		for(int m=0;m<i-1;m++)
		{
			printf("content:%s\n",myargv[m]);
			if(strcmp(myargv[m],"exit")==0)
			{
				return 0;
			}
		}
		//打印下字符串信息
		int id=fork();
		if(id==0)
		{
			if(strcmp(myargv[0],"cd")==0)
			{
				chdir(myargv[1]);
			}//实现改变路径的功能
			else if(strcmp(myargv[0],"cat")==0)
			{
				    FILE *ifp;
 					FILE *ofp;
 					if(i>3)
 					{
    				if(((ifp = fopen(myargv[1], "r")) != NULL) && ((ofp = fopen(myargv[2], "a")) != NULL))
  						{
      						filecopy(ifp, ofp);
   							fclose(ifp);
   							fclose(ofp);
  						}
   					else
  						{
      						printf("ERROR: can't open file\r\n");
    						exit(1);
  						}
  					}//实现文件的复制功能
					 else
 						{
 						 	filecopy(ifp, stdout);
    						fclose(ifp);
 						}
 						//将文件内容显示出来
			}
			else
			{
				//执行替换操作
				execvp(myargv[0],myargv);
				perror("Error");
				exit(1);
			}
		}
		else
		{
			 if(strcmp(myargv[0],"exit")==0)
			{
				printf("*********************bye*********************\n");
				return 0;
			}
			printf( "father\r\n");
			wait ( NULL );
			//结束程序
         	//父进程等待子进程返回
		}

	}
	return 0;
}
