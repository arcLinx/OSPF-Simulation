#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include "port.h"

#define TCP_PORT 25516
#define UDP_PORT_B 22516
void my_error(char *msg)
{
	perror(msg);
	exit(1);
}
/*--- Data structure for storing the details ----*/

typedef struct srvB{
	char node_name[10];
	int link_cost;
}srvrBdat;


/*---- Peer address and IP address ----*/
char peer_ip_address[20];
int peer_port;

/*---- host address and IP address ----*/
struct in_addr **host_addr;
char host_ip[100];

/*----Global socket structure ----*/
struct sockaddr_in myaddr_TCP;
struct sockaddr_in myaddr_UDP;
struct sockaddr_in peeraddr;


/*--- Data structure for storing the neighbouring information ----*/
typedef struct server{
	struct server *next;
	srvrBdat *data;
}serverB;

/*---- Head Node ----*/
serverB head;

/*---- Global Variables ----*/
char sendbuf[1024];
char recvbuf[1024];

/*---- Append the node to the linked list ----*/
void append(serverB *node)
{
	serverB *tempnode;
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
/*---- Server TCP Socket ----*/
void create_tcp_socket()
{
	int tcpfd,len;
	
	//struct sockaddr_in peeraddr;
	//struct hostent* hp;

	//char buf[20];// = "Accept my request p";

	tcpfd = socket(AF_INET, SOCK_STREAM,0);
	if(tcpfd < 0)
		my_error("Could not establish a socket\n");
	memset(&myaddr_TCP,0,sizeof(myaddr_TCP));
	
	myaddr_TCP.sin_family = AF_INET;
	
	myaddr_TCP.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	myaddr_TCP.sin_port = htons(TCP_PORT);
	
	len = sizeof(struct sockaddr_in);
	usleep(100000);
	
	if( connect(tcpfd,(struct sockaddr*)&myaddr_TCP,len) < 0)
		my_error("Failed to connect\n");
		
	serverB *tempnode;
	tempnode = &head;
	char tempbuf[10];
	while(tempnode->next != NULL ){
		
		strcpy(sendbuf,tempnode->data->node_name);
		sprintf(tempbuf,"%d", tempnode->data->link_cost);
		strcat(sendbuf,"  ");
		strcat(sendbuf,tempbuf);
		usleep(100000);
		if(send(tcpfd,sendbuf,20,0) < 0)
			my_error("Failed to send\n");
		usleep(100000);
		tempnode = tempnode->next;
		
	}
	if(send(tcpfd,"exit",10,0) < 0)
		my_error("Failed to send\n");
	close(tcpfd);
}

/*---- Server UDP socket----*/
void sever_UDP_Static(int broadcaston)
{
	
		int udpfd,fromlen, n,len;
		//struct sockaddr_in myaddr;
		//struct sockaddr_in peeraddr;
		//struct hostent *hp;
		
		udpfd = socket(PF_INET,SOCK_DGRAM,0);
		if(udpfd < 0)
			my_error("Could not establish a socket\n");


		len = sizeof(myaddr_UDP);
		bzero(&myaddr_UDP, len);
		myaddr_UDP.sin_family = AF_INET;
		myaddr_UDP.sin_port = htons(UDP_PORT_B);
		myaddr_UDP.sin_addr.s_addr = INADDR_ANY;
		
		if( bind(udpfd, (struct sockaddr*)&myaddr_UDP, (int)sizeof(myaddr_UDP)) < 0)
			my_error("Binding unsuccessfull\n");
		
		int flag = 0;
		fromlen = (sizeof(peeraddr));
		while(1){
			n = recvfrom(udpfd,(void *)recvbuf,sizeof(recvbuf),0,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
			//sendto(udpfd,"",1,0,(struct sockaddr *) &peeraddr,fromlen);
			//usleep(1000000);
			if(n<0)
				my_error("Failed to receive\n");
				
			/*---- Get the peer address to be used ----*/
			getpeername(udpfd, (struct sockaddr*)&peeraddr,(socklen_t *) &fromlen);
			struct sockaddr_in *s = (struct sockaddr_in *)&peeraddr;
			if(broadcaston == 1)
				peer_port = ntohs(s->sin_port);
			else
				server_tcp_dynamic_port = ntohs(s->sin_port);
			inet_ntop(AF_INET, &s->sin_addr, peer_ip_address, sizeof peer_ip_address);	
							
			if(broadcaston != 0){
				if(flag ==0){
					printf("The server B has received the network topology from the Client with UDP port number %d and %s IP address as follows:\n",peer_port,peer_ip_address);
					printf("Edge------Cost\n");
					flag =1;
				}
				char name[10],link[10];
				if(strcmp(recvbuf,"exit") == 0)
					break;
				sscanf(recvbuf,"%s\t%s",name,link);
				
				printf("%s\t%s\n",name,link);		
			}
			else 
				break;
		}
		close(udpfd);
	
}

/*---- Print the elements of the linked list ----*/
/*void print_list(serverB hd)
{
	serverB *tempnode;
	tempnode = &hd;
	while(tempnode->next != NULL ){
		printf("The %s has a link cost of %d\n",tempnode->data->node_name, tempnode->data->link_cost);
		tempnode = tempnode->next;
	}
}
*/
/*---- Read the file ServerA.txt as it is ----*/

void serverBootUp()
{
	char buf[1024];
	FILE *fp = NULL;
	fp = fopen("serverB.txt","r");
	head.next = NULL;
	head.data = NULL;
	struct hostent *hp;
	 if ((hp = gethostbyname( "localhost" ) ) == NULL) 
		my_error("gethostbyname");
    
     host_addr = (struct in_addr **) hp->h_addr_list;
    int i;
    for(i = 0; host_addr[i] != NULL; i++) 
		strcpy(host_ip , inet_ntoa(*host_addr[i]) );
		
	printf("The Server B is up and running\n");
	printf("The Server B has the following neighbor information:\n");
	printf("Neighbor------Cost\n");
	while(fgets(buf,sizeof(buf),fp) != NULL)
	{
		serverB *b = (serverB *)malloc(sizeof(serverB));
		srvrBdat *bdat = (srvrBdat *)malloc(sizeof(srvrBdat));
		sscanf(buf,"%s\t%d", bdat->node_name,&bdat->link_cost);
 		b->data = bdat;
 		printf("%s\t%d\n",b->data->node_name, b->data->link_cost);
 		append(b);

	}
}
int server_tcp_dynamic_port;
int main()
{
	
	serverBootUp();
	//print_list(head);
	sever_UDP_Static(0);
	create_tcp_socket();
	printf("The Server B finishes sending its neighbor information to the Client with TCP port number %d and IP address %s\n",TCP_PORT,peer_ip_address);
	printf("For this connection with the Client, the Server B has TCP port number %d and IP address %s\n",server_tcp_dynamic_port,host_ip);
	sever_UDP_Static(1);
	printf("For this connection with Client, the Server B has UDP port number %d and IP address %s\n",UDP_PORT_B,host_ip);
	
	return 0;
}
