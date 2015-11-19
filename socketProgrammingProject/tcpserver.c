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
	sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		my_error("Could not establish a socket\n");
	len = sizeof(myaddr);
	bzero(&myaddr, len);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(atoi(argv[1]));
	
	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	fromlen = (sizeof(myaddr));
	char ipstr[20];
	int port;
	//struct sockaddr_in *s = (struct sockaddr_in *)&myaddr;
	
	//inet_addr("10.123.66.117");
	//bcopy((char *)hp->h_addr,(char*)&myaddr.sin_addr,hp->h_length);     // short, network byte order
	//myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(bind(sockfd, (struct sockaddr*)&myaddr, (int)sizeof(myaddr)) < 0)
		my_error("Binding unsuccessfull\n");
	
	
	if( listen(sockfd,10) < 0)
		my_error("Failed to listen\n");
		
	int childfd;
	
	childfd = accept(sockfd,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
	
	int check = getsockname(sockfd,(struct sockaddr*)&myaddr,(socklen_t *)&fromlen);
	
	port = ntohs(myaddr.sin_port);
	inet_ntop(AF_INET, &myaddr.sin_addr, ipstr, sizeof ipstr);
	
	printf("Peer IP address: %s\n", ipstr);
	printf("Peer port      : %d\n", port);
	
	
//	getsockname(sockfd, struct sockaddr *addr, socklen_t *addrlen);
	
	if(childfd < 0)
		my_error("Failed to accept\n");
	
	n = recv(childfd,buf,20,0);
	if(n<0)
		my_error("Failed to receive\n");
	printf("%s\n",buf);
	//send(childfd, "Got it\n", sizeof(buf),0);

	close(childfd);

	close(sockfd);
	return 0;
}

