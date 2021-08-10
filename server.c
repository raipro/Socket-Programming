#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define PORT 8000
long long int bsize =1e4;
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;  
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1000002];
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	if(bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Connection made\n");
	fflush(NULL);
	while(1)
	{
		for(int k=0;k<bsize;k++)
		{
			buffer[k] = '\0';
		}
	//	printf("blyat");
		valread = read(new_socket , buffer, bsize);
	//	printf("%s\n",buffer);
		buffer[valread]='\0';
		if(buffer[0]=='\0' || (!strcmp(buffer,"exit")))
			break;
		//printf("command read as %s\n",buffer);
		fflush(NULL);
		int fd = open(buffer, O_RDONLY);
		if(fd<0)
		{
			strcpy(buffer,"no");
			send(new_socket , buffer , strlen(buffer) , 0 );
			read(new_socket,buffer,4);
		//	perror("server file");
			//exit(EXIT_FAILURE);
			continue;
		}
		else
		{
		//	printf("Hell yeah\n");
			fflush(NULL);
			strcpy(buffer,"yes");
			send(new_socket , buffer , strlen(buffer) , 0 );
			read(new_socket,buffer,4);
			int size;
			if((size=lseek(fd,0,SEEK_END))==-1)
			{
				perror("size calculation");
				return -1;    
			}
			lseek(fd,0,SEEK_SET);
			sprintf(buffer,"%d",size);
			send(new_socket , buffer , strlen(buffer) , 0 );
			int rev=read(new_socket , buffer, bsize);
			buffer[rev]='\0';
			if(!strcmp(buffer,"size received"))
			{
				while(size>bsize)
				{
                                        read(fd,buffer,bsize);
                                        buffer[bsize]='\0';
					send(new_socket , buffer , bsize , 0 );
                                        read(new_socket , buffer, bsize);
					size-=bsize;
				}
                                read(fd,buffer,size);
                                send(new_socket , buffer ,size , 0 );
                                read(new_socket , buffer, bsize);
				size=0;
			}
		}
	}
//	printf("connection to the client closed");
	}
	return 0;
}
