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
	//Create a client UDP socket

	int sockfd, fromlen, n,len;
	struct sockaddr_in myaddr;
	//struct sockaddr_in peeraddr;
	struct hostent* hp;

	char buf[20];// = "Accept my request p";

	sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd < 0)
		my_error("Could not establish a socket\n");
	memset(&myaddr,0,sizeof(myaddr));
	
	myaddr.sin_family = AF_INET;
	

	
	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	myaddr.sin_port = htons(atoi(argv[1]));
	
	len = sizeof(struct sockaddr_in);
	// send the request on the created socket
	if( connect(sockfd,(struct sockaddr*)&myaddr,len) < 0)
		my_error("Failed to connect\n");
	
	fgets(buf,20,stdin);
	if(send(sockfd,buf,20,0) < 0)
		my_error("Failed to send\n");
	
	

	close(sockfd);
	
	return 0;
}
