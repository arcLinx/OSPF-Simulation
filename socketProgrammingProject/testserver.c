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


int main()

{
	
	//Initialzing the address of the sendere and receiver
	//struct addrinfo* pointer_addr_struct;
	struct addrinfo my_add;
	//struct addrinfo socktype;
	struct addrinfo *ptr_addr;
	struct sockaddr_in IP;
	//struct sockaddr_in my_addr; 
	struct sockaddr *my_addr,*peer_addr;
	struct sockaddr my_addres;
	struct sockaddr_in myaddr;
	struct sockaddr_storage praddr;
	//struct sockaddr_storage* peer_addr =NULL;
	socklen_t len;
	//establish the socket
	int sockfd = NULL;
	
		
	//Initialize the socket with all the addresses
	
	/* Structures within the socket.h
	 * addrinfo -> this is the structure which holds the Information of all the addresses and protocol family, type -> we have to set family, type and addres
	 * sockaddr -> this is a structure which holds the type of family used and the address and the port number of the server/client (on a char array) 
	 * sockaddr_in -> this is the structure which holds the family type, port number to be assigned and zero padding to match with the clients data.
	 * in_addr -> this has a field to set the source address of the which is of type usigned 32 bit integer
	*/
	
	memset(&my_add,0,sizeof(my_add));
	my_add.ai_family = AF_INET;
	my_add.ai_socktype = SOCK_DGRAM;
	//my_addres.sin_port = 12;
	my_add.ai_flags = AI_PASSIVE;
	//getaddrinfo(NULL, "1234", &my_add, &ptr_addr);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(1234);     // short, network byte order
	myaddr.sin_addr.s_addr = inet_addr("10.123.67.64");
	memset(myaddr.sin_zero, '\0', sizeof myaddr.sin_zero);
	
	
	sockfd = socket(my_add.ai_family, my_add.ai_socktype, 0);
	//Bind the socket == assign an address to the socket
	bind(sockfd, (struct sockaddr*)&myaddr, (int)sizeof(myaddr));
	printf("Binded successfully\n");
	
	printf("sa data = %d\n",myaddr.sin_addr.s_addr);

	//Recevie the medsage
	char buf[20];
	
	recvfrom(sockfd,(void *)buf,(size_t)sizeof(buf),0,(struct sockaddr*)&praddr,(socklen_t *)(sizeof(praddr)));

	printf(" The input I received is *** %s *** \n", buf);

	close(sockfd);
	return 0;
}
