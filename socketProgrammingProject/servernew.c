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
	struct hostent *hp;
	char buf[1024];
	sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
		my_error("Could not establish a socket\n");


	len = sizeof(myaddr);
	bzero(&myaddr, len);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(atoi(argv[1]));
	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	if( bind(sockfd, (struct sockaddr*)&myaddr, (int)sizeof(myaddr)) < 0)
		my_error("Binding unsuccessfull\n");
	
	
	fromlen = (sizeof(peeraddr));
	
	n = recvfrom(sockfd,(void *)buf,sizeof(buf),0,(struct sockaddr*)&peeraddr,&fromlen);
	if(n<0)
		my_error("Failed to receive\n");
	
	printf("%s\n",buf);
	sendto(sockfd, "Got it\n", sizeof(buf),0,(struct sockaddr *) &peeraddr,fromlen);

	

	close(sockfd);
	return 0;
}
	


	