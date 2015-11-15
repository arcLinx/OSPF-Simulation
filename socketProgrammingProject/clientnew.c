#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define UDP_PORT 1234

void my_error(char *msg)
{
	perror(msg);
	exit(1);
}
typedef struct pass{
	char name[10];
	int age;
}pass1;


int main(int argc, char* argv[])
{
	int sockfd, fromlen, n,len;
	struct sockaddr_in myaddr;
	struct sockaddr_in peeraddr;
	//struct hostent* hp;

	char buf[20];
	pass1 p1;

	sockfd = socket(AF_INET, SOCK_DGRAM,0);
	if(sockfd < 0)
		my_error("Could not establish a socket\n");

	memset(&myaddr,0,sizeof(myaddr));
	
	myaddr.sin_family = SOCK_DGRAM;

	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	myaddr.sin_port = htons(UDP_PORT);
	fgets(buf,sizeof(buf),stdin);
	len = sizeof(struct sockaddr_in);
	
	if(sendto(sockfd, (void *)buf, sizeof(buf),0,(struct sockaddr *) &myaddr,len) < 0)
		my_error("Failed to send\n");
	memset(buf,0,sizeof(buf));
	n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
	
	sscanf(buf,"%s\t%d",p1.name,&p1.age);
	printf("Name: %s\t Age: %d\n",p1.name,p1.age); 
	

	close(sockfd);
	
	return 0;
}
