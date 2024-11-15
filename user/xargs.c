#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUFSIZE 512

int main(int argc, char *argv[])
{
	char buffer[BUFSIZE];
	char c;
	char *args[MAXARG];
	int argi;

	int newLength=argc-1, oldLength=argc-1,index=0;

	//Sao chep cac doi so co dinh
	for(argi=1;argi<argc;argi++)
	{
		args[argi-1]=argv[argi];
	}

	while(read(0,&c,1)==1)
	{
		switch(c)
		{
			case ' ':
				buffer[index]='\0';
				args[newLength]=malloc(index+1);
				strcpy(args[newLength++],buffer);
				index=0;
				break;
			case '\n':
				buffer[index]='\0';
				args[newLength]=malloc(index+1);
				strcpy(args[newLength++],buffer);
				index=0;

				args[newLength]=0;

				//Tao fork
				int fd=fork();
				if(fd<0)
				{
					fprintf(2,"xargs: fork error\n");

				}
				else if(fd==0)
				{
					//tien trinh con
					exec(args[0],args);

					//neu exec khong tao ra mot tien trinh moi thanh cong thi no se chay xuong day
					fprintf(2,"xargs: exec error\n");
					exit(1);
				}
				else
				{
					//tien trinh cha doi tien trinh con ket thuc moi thuc hien tiep
					wait(0);
					//giai phong bo nho cho cac lan tiep theo thuc hien
					for(int i=oldLength;i<newLength;i++)
					{
						free(args[i]);
					}
					newLength=oldLength;
				}
				break;
			default:
				buffer[index++]=c;
				break;
		}
		}

	
	}
	

