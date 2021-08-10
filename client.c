#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <arpa/inet.h>
#define PORT 8000
int bsize = 1e4;
int main(int argc, char const *argv[])
{
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char buffer[1000002] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	while(1)
	{
		char *buf = NULL; 
		size_t asize = 0;
		printf("client>");
		getline(&buf,&asize,stdin);
		if(buf[strlen(buf)-1] == '\n')
			buf[strlen(buf)-1] = '\0';
		int j1=0;
		int j=0;
		char str[100][1000];
		memset(str,'\0',100000);
		for(int i=0;i<strlen(buf);i++)
		{
			if(buf[i]==' '||buf[i]=='\t')
			{
				continue;
			}
			str[j][j1] = buf[i];
			j1++;
			if(buf[i+1]==' '||buf[i+1]=='\t')
			{
				str[j][j1] = '\0';
				j++;	
				j1=0;
			}
		}
		if(!strcmp(str[0],"exit"))
		{
			send(sock , str[0] , strlen(str[0]) , 0 );
			break;
		}
		else if(!strcmp(str[0],"get"))
		{
			for(int i=1;str[i][0]!='\0';i++)
			{
			//	printf("blyat");	
				send(sock , str[i] , strlen(str[i]) , 0 ); // send the message.
				for(int k=0;k<bsize;k++)
                		{
                        		buffer[k] = '\0';
                		}
				int rev = read(sock,buffer,bsize);
				buffer[rev] = '\0';
				//printf("yes or no is %s\n",buffer);
				fflush(NULL);
				send(sock,"done",4,0);
				if(!strcmp(buffer,"yes"))
				{
				//	printf("In yes\n");
					fflush(NULL);
					int fd = open(str[i],O_CREAT | O_WRONLY | O_TRUNC , 0666);
					if(fd<0)
					{
						perror("client file");
					//	exit(EXIT_FAILURE);
					        continue;
					}
					for(int k=0;k<bsize;k++)
                				{
                        				buffer[k] = '\0';
                				}
					valread = read( sock , buffer, bsize);
					buffer[valread]='\0';
					int size = atoi(buffer);
					int size1 = size;
					strcpy(buffer,"size received");
					//printf("%s\n",buffer);
					char kr[2];
					fflush(NULL);
					send(sock , buffer , strlen(buffer) , 0 );
					while(size>bsize)
					{
						for(int k=0;k<bsize;k++)
                				{	
                        				buffer[k] = '\0';
                				}
                                                valread = read( sock , buffer, bsize);
                                                buffer[bsize]='\0';
						write(fd,buffer,bsize);
                                                strcpy(buffer,"bits transferred");
                                                send(sock , buffer , strlen(buffer) , 0 );
						size=size-bsize;
						float x = ((float)((size1-size)))/((float)size1);
						write(1,"\r",1);
						x=x*10;
						kr[0]=(char)((((int)x)%10)+'0');
						kr[1]='\0';
						write(1,kr,1);
						x=x*10;
						kr[0]=(char)((((int)x)%10)+'0');
						write(1,kr,1);
						kr[0]='.';
						write(1,kr,1);
						x=x*10;
                                                kr[0]=(char)((((int)x)%10)+'0');
                                                write(1,kr,1);
						x=x*10;
                                                kr[0]=(char)((((int)x)%10)+'0');
                                                write(1,kr,1);
						kr[0]='%';
						write(1,kr,1);
					}
					for(int k=0;k<bsize;k++)
                			{
                        			buffer[k] = '\0';
                			}
                                        valread = read( sock , buffer, size);
                                        buffer[size]='\0';
					write(fd,buffer,size);
                                        strcpy(buffer,"bits transferred");
                                        send(sock , buffer , strlen(buffer) , 0 );
                                        size=size-size;
					write(1,"\r",1);
					write(1,"100.00%\n",8);
    					//write(1,buf1,strlen(buf1));
				}
				else
				{
					printf("invalid file\n");
				}
			}
		}
		else
		{
			printf("invalid command\n");
			continue;
		}
	}
	printf("connection with the server is closed");
	return 0;
}
