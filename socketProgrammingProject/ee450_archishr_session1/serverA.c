#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "port.h"
#define TCP_PORT 25516
#define UDP_PORT_A 21516

int server_tcp_dynamic_port;

/*--- Data structure for storing the details ----*/
typedef struct srvA{
	char node_name[10];
	int link_cost;
}srvrAdat;

/*---- Peer address and IP address ----*/
char peer_ip_address[20];
int peer_port;


/*---- host address and IP address ----*/
struct in_addr **host_addr;
char host_ip[100];

/*---- Display the error message and terminate the user process----*/
void my_error(char *msg)
{
	perror(msg);
	exit(1);
}
/*----Global socket structure ----*/
struct sockaddr_in myaddr_TCP;
struct sockaddr_in myaddr_UDP;
struct sockaddr_in peeraddr;

/*---- Global Variables ----*/
char sendbuf[1024];
char recvbuf[1024];
char serverA_IP[20];

/*--- Linked list Data structure ----*/
typedef struct server{
	struct server *next;
	srvrAdat *data;
}serverA;

/*---- Head Node ----*/
serverA head;

/*---- Append the node to the linked list ----*/
void append(serverA *node)
{
	serverA *tempnode;
	tempnode = &head;
	if(tempnode->next == NULL){
		tempnode->next = node;
		tempnode->data = node->data;
		node->next = NULL;
	}
	else{
		while(tempnode->next != NULL){
			
			tempnode = tempnode->next;
		}	
		tempnode->data = node->data;
		tempnode->next = node;
		node->next = NULL;
	}
	
}
/*---- Server TCP Socket used to send data to the client----*/
void create_tcp_socket()
{
	int tcpfd,len;
	struct hostent* hp;
	
	/*---- Create the TCP socket descriptor----*/
	tcpfd = socket(AF_INET, SOCK_STREAM,0);
	if(tcpfd < 0)
		my_error("Could not establish a socket\n");
		
	/*---- Initialize the socket address structure----*/
	memset(&myaddr_TCP,0,sizeof(myaddr_TCP));
	
	/*---- Assign Local Host IP address and port number ----*/
	 if ( (hp = gethostbyname( "localhost" ) ) == NULL) 
		my_error("gethostbyname");
	host_addr = (struct in_addr **) hp->h_addr_list;
    int i;
    for(i = 0; host_addr[i] != NULL; i++) 
		strcpy(host_ip , inet_ntoa(*host_addr[i]) );
    myaddr_TCP.sin_family = AF_INET;
	myaddr_TCP.sin_addr.s_addr = inet_addr(host_ip);
	myaddr_TCP.sin_port = htons(TCP_PORT);
	len = sizeof(struct sockaddr_in);
	
	/*----This is for syncing the server and client communication ----*/
	usleep(1000000);
	
	/*---- Establish a SOCK_STREAM connection with the client who is blocked on the accept call----*/
	if( connect(tcpfd,(struct sockaddr*)&myaddr_TCP,len) < 0)
		my_error("Failed to connect\n");
		
	serverA *tempnode;
	tempnode = &head;
	char tempbuf[10];
	
	/*---- Iteratively send the neighbor and cost information to the client who is blocked on recv call----*/
	while(tempnode->next != NULL ){
		
		strcpy(sendbuf,tempnode->data->node_name);
		sprintf(tempbuf,"%d", tempnode->data->link_cost);
		strcat(sendbuf,"  ");
		strcat(sendbuf,tempbuf);
		
		/*----This is for syncing the server and client communication ----*/
		usleep(1000000);
		
		/*---- Send the neighbor cost information to client ----*/
		if(send(tcpfd,sendbuf,20,0) < 0)
			my_error("Failed to send\n");
			
		/*----This is for syncing the server and client communication ----*/
		usleep(100000);
		tempnode = tempnode->next;
		
	}
	/*---- Send Exit to close connection ----*/
	if(send(tcpfd,"exit",10,0) < 0)
		my_error("Failed to send\n");
	close(tcpfd);
}

/*---- ServerA UDP socket to receive data from client----*/
void sever_UDP_Static(int broadcaston)
{
	
		int udpfd,fromlen, n,len;
		
		/*---- Create a UDP socket descriptor----*/
		udpfd = socket(PF_INET,SOCK_DGRAM,0);
		if(udpfd < 0)
			my_error("Could not establish a socket\n");
			
		/*---- Initialize the socket address structure----*/
		len = sizeof(myaddr_UDP);
		bzero(&myaddr_UDP, len);
		myaddr_UDP.sin_family = AF_INET;
		myaddr_UDP.sin_port = htons(UDP_PORT_A);
		myaddr_UDP.sin_addr.s_addr = INADDR_ANY;
		
		/*---- Bind the server A to the  static UDP_PORT number and localhost IP address of the created UDP socket descriptor----*/
		if( bind(udpfd, (struct sockaddr*)&myaddr_UDP, (int)sizeof(myaddr_UDP)) < 0)
			my_error("Binding unsuccessfull\n");
		
		fromlen = (sizeof(peeraddr));
					
		int flag =0;
		while(1){
			/*---- Wait for the clients to accept datagram from the client----*/
			n = recvfrom(udpfd,(void *)recvbuf,sizeof(recvbuf),0,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);

			/*---- Get the peer address to be used ----*/
			getpeername(udpfd, (struct sockaddr*)&peeraddr,(socklen_t *) &fromlen);
			struct sockaddr_in *s = (struct sockaddr_in *)&peeraddr;
			if(broadcaston == 1){
				peer_port = ntohs(s->sin_port);
				FILE * fp= NULL;
				fp = fopen("UDP_port.txt","w");
				fprintf(fp, "%d\n",peer_port);
				fclose(fp);
			}
			else
				server_tcp_dynamic_port = ntohs(s->sin_port);
			inet_ntop(AF_INET, &s->sin_addr, peer_ip_address, sizeof peer_ip_address);
			
			if(n<0)
				my_error("Failed to receive\n");
				
			if(broadcaston != 0){
				if(flag ==0){
					printf("\nThe server A has received the network topology from the Client with UDP port number %d and %s IP address as follows:\n",peer_port,peer_ip_address);
					printf("\nEdge----Cost\n");
					flag =1;
				}
				char name[10],link[10];
				if(strcmp(recvbuf,"exit") == 0)
					break;
				sscanf(recvbuf,"%s\t%s",name,link);
				
				printf("%s       %s\n",name,link);		
			}
			else 
				break;
		}
		close(udpfd);
	
}
/*---- Read the file serverA.txt as mentioned in the directory and print the neighbor cost information----*/
void serverBootUp()
{
	char buf[1024];
	
	FILE *fp = NULL;
	fp = fopen("serverA.txt","r");
	head.next = NULL;
	head.data = NULL;
	
	printf("The Server A is up and running\n");
	printf("The Server A has the following neighbor information:\n");
	printf("Neighbor------Cost\n");
	while(fgets(buf,sizeof(buf),fp) != NULL)
	{
		serverA *a = (serverA *)malloc(sizeof(serverA));
		srvrAdat *adat = (srvrAdat *)malloc(sizeof(srvrAdat));
		sscanf(buf,"%s\t%d", adat->node_name,&adat->link_cost);
 		a->data = adat;
 		printf("%s       %d\n",adat->node_name, adat->link_cost);
 		append(a);
	}
}
/*---- Main function of server A ----*/
int main()
{
	/*---- Boot up the server and read the contents of the file and print them----*/
	serverBootUp();
	
	/*---- Start a UDP socket and block on the receive call to hear from the client----*/
	sever_UDP_Static(0);
	
	/*---- Once the client is ready to receive from the server, start a TCP socket and send data to the client----*/
	create_tcp_socket();
	
	/*---- On screen Messages ----*/
	printf("\nThe Server A finishes sending its neighbor information to the Client with TCP port number %d and IP address %s\n",TCP_PORT,peer_ip_address);
	printf("For this connection with the Client, the Server A has TCP port number %d and IP address %s\n",server_tcp_dynamic_port,host_ip);
	
	/*---- Start a UDP socket and block on the receive call to hear from the client about the network Topology---*/
	sever_UDP_Static(1);
	
	/*---- On screen Messages ----*/
	printf("For this connection with Client, the Server A has UDP port number %d and IP address %s\n",UDP_PORT_A,host_ip);
	
	return 0;
}
