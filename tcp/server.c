#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int* newsockets;
int clientnum;

typedef struct mysocket
{
	int sockfd;
	int newsockfd;
} mysockfd_t;

void* Process(void* mySocket){
	mysockfd_t mysockfd = *(mysockfd_t*)mySocket;
	char line[1000];
	int n;
	while((n = read(mysockfd.newsockfd, line, 999)) > 0)
	{
        	for (int i = 0; i < clientnum; ++i)
		{
        		if (newsockets[i] != mysockfd.newsockfd)
			{
	        		if((n = write(newsockets[i], line,strlen(line)+1)) < 0)
	        		{
	        			perror(NULL);
		        		close(mysockfd.sockfd);
	        			close(mysockfd.newsockfd);
	       		 		exit(1);
	        		}
        		}
        	}
    	}
}

int main()
{
	mysockfd_t mysockfd;
	clientnum = 0;
	newsockets = calloc(1000, sizeof(int));
	int clilen;
	int n;
	char line[1000];
	struct sockaddr_in servaddr, cliaddr;
	if((mysockfd.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror(NULL);
        	exit(1);
        }
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family= AF_INET;
	servaddr.sin_port= htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(mysockfd.sockfd, (struct sockaddr *) &servaddr,sizeof(servaddr)) < 0){
        	perror(NULL);
        	close(mysockfd.sockfd);
        	exit(1);
	}	
	if(listen(mysockfd.sockfd, 5) < 0){
        	perror(NULL);
        	close(mysockfd.sockfd);
        	exit(1);
	}
    	while(1)
	{
    		pthread_t thid;
        	clilen = sizeof(cliaddr);
        	if((mysockfd.newsockfd = accept(mysockfd.sockfd,(struct sockaddr *) &cliaddr, &clilen)) < 0){
			perror(NULL);
			close(mysockfd.sockfd);
			exit(1);
        	}
        	newsockets[clientnum] = mysockfd.newsockfd;
		clientnum++;
  		pthread_create(&thid, (pthread_attr_t *)NULL, Process, &mysockfd);
	}
}
