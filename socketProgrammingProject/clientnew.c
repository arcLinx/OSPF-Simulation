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

void my_error(char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char* argv[])
{
	int sockfd, fromlen, n,len;
	struct sockaddr_in myaddr;
	struct sockaddr_in peeraddr;
	struct hostent* hp;

	char buf[20];

	sockfd = socket(AF_INET, SOCK_DGRAM,0);
	if(sockfd < 0)
		my_error("Could not establish a socket\n");

	memset(&myaddr,0,sizeof(myaddr));
	
	myaddr.sin_family = SOCK_DGRAM;

	myaddr.sin_addr.s_addr = inet_addr("10.123.66.117");
	
	myaddr.sin_port = htons(atoi(argv[1]));
	fgets(buf,sizeof(buf),stdin);
	len = sizeof(struct sockaddr_in);
	
	if(sendto(sockfd, (void *)buf, sizeof(buf),0,(struct sockaddr *) &myaddr,len) < 0)
		my_error("Failed to send\n");
	memset(buf,0,sizeof(buf));
	n = recvfrom(sockfd,(void *)buf,sizeof(buf),0,(struct sockaddr*)&peeraddr,&fromlen);
	if(n<0)
		my_error("Failed to receive\n");
	printf("%s\n",buf);
	

	close(sockfd);
	
	return 0;
}