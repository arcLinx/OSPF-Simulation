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

#define TCP_PORT 25516
#define UDP_PORT_D 24516


/*--- Data structure for storing the details ----*/

typedef struct srvD{
	char node_name[10];
	int link_cost;
}srvrDdat;

/*--- Data structure for storing the neighbouring information ----*/
typedef struct server{
	struct server *next;
	srvrDdat *data;
}serverD;

/*---- Head Node ----*/
serverD head;


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


/*---- Append the node to the linked list ----*/
void append(serverD *node)
{
	serverD *tempnode;
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
	usleep(1000);
	// send the request on the created socket
	if( connect(tcpfd,(struct sockaddr*)&myaddr_TCP,len) < 0)
		my_error("Failed to connect\n");
		
	serverD *tempnode;
	tempnode = &head;
	char tempbuf[10];
	while(tempnode->next != NULL ){
		
		strcpy(sendbuf,tempnode->data->node_name);
		sprintf(tempbuf,"%d", tempnode->data->link_cost);
		strcat(sendbuf,"  ");
		strcat(sendbuf,tempbuf);
		usleep(1000);
		if(send(tcpfd,sendbuf,20,0) < 0)
			my_error("Failed to send\n");
		usleep(1000);
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
		myaddr_UDP.sin_port = htons(UDP_PORT_D);
		myaddr_UDP.sin_addr.s_addr = INADDR_ANY;
		
		if( bind(udpfd, (struct sockaddr*)&myaddr_UDP, (int)sizeof(myaddr_UDP)) < 0)
			my_error("Binding unsuccessfull\n");
		
		
		fromlen = (sizeof(peeraddr));
		
		while(1){
			n = recvfrom(udpfd,(void *)recvbuf,sizeof(recvbuf),0,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
			//sendto(udpfd,"",1,0,(struct sockaddr *) &peeraddr,fromlen);
			//usleep(1000000);
			if(n<0)
				my_error("Failed to receive\n");
				
			if(broadcaston != 0){
				
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
/*---- Read the file ServerA.txt as it is ----*/

void serverBootUp()
{
	char buf[1024];
	FILE *fp = NULL;
	fp = fopen("serverD.txt","r");
	head.next = NULL;
	head.data = NULL;
			
	while(fgets(buf,sizeof(buf),fp) != NULL)
	{
		serverD *d = (serverD *)malloc(sizeof(serverD));
		srvrDdat *ddat = (srvrDdat *)malloc(sizeof(srvrDdat));
		
 		sscanf(buf,"%s\t%d", ddat->node_name,&ddat->link_cost);
 		printf("%s \t %d \n",ddat->node_name, ddat->link_cost);
 		d->data = ddat;
 		
 		append(d);
		
	}
}



int main()
{
	
	serverBootUp();
	sever_UDP_Static(0);
	create_tcp_socket();
		
	printf("Edge----Cost\n");
	sever_UDP_Static(1);
	return 0;
}
