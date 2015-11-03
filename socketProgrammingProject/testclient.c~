#include<stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	//Create a client UDP socket

	struct addrinfo my_add;
	struct addrinfo peer_add;
	struct addrinfo *ptr_addr;
	struct sockaddr *my_addr,*peer_addr;
	struct sockaddr my_addres;
	struct sockaddr_in myaddr,peeraddr;
	int sockfd;
	char buf[20] = "Accept my request p";

	sockfd = socket(AF_INET, SOCK_DGRAM,0);
	memset(&my_add,0,sizeof(my_add));
	my_add.ai_family = AF_INET;
	my_add.ai_socktype = SOCK_DGRAM;
		
	//getaddrinfo("10.123.67.63", "1234", &peer_add, &ptr_addr);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(1234);     // short, network byte order
	myaddr.sin_addr.s_addr = inet_addr("10.123.67.64");
	memset(myaddr.sin_zero, '\0', sizeof myaddr.sin_zero);
	

	// send the request on the created socket
	sendto(sockfd, (void *)buf, sizeof(buf),0,(struct sockaddr *) &myaddr,(socklen_t) sizeof(myaddr));
	
	

	close(sockfd);
	
	return 0;
}
