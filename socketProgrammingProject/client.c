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

/*---- All the static ports in the network ----*/

#define TCP_PORT 25516
#define UDP_PORT_A 21516
#define UDP_PORT_B 22516
#define UDP_PORT_C 23516
#define UDP_PORT_D 24516

/*--- Data structure to store the server neighbouts ----*/
void my_error(char *msg)
{
	perror(msg);
	exit(1);
}
/*--- details ---*/
typedef struct client{
	char node_name[10];
	int link_cost;
}clientdet;

/*--- Client List ---*/
typedef struct allserver{
	struct allserver *next;
	clientdet *data;
}clientlist;

/*--- Global head ---*/
clientlist head;

/*---- Global variables*/
int adjacency[4][4];
char adjacencystring[5];
char recvbuf[1024];
char sendbuf[1024];
int i=0;
struct sockaddr_in myaddr,peeraddr;
/*Global socket parameters*/


/*--- Append the data structure ---*/
void append(clientlist *node)
{
	clientlist *tempnode;
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
/*---- Load Adjacency Matrix ----*/
void load_adjacency(int i,clientdet *clist)
{
	if(strcmp(clist->node_name,"serverA") == 0 )
		adjacency[i][0] = clist->link_cost;
	else if (strcmp(clist->node_name,"serverB") == 0)
		adjacency[i][1] = clist->link_cost;
	else if (strcmp(clist->node_name,"serverC") == 0)
		adjacency[i][2] = clist->link_cost;
	else if (strcmp(clist->node_name,"serverD") == 0)
		adjacency[i][3] = clist->link_cost;
}


/*---- TCP Socket ---*/
void create_tcp_socket()
{
	int fromlen,tcpfd, n,len;
	//struct sockaddr_in myaddr;
	//struct sockaddr_in peeraddr;
	//struct hostent *hp;
	//char buf[1024];
	tcpfd = socket(PF_INET,SOCK_STREAM,0);
	if(tcpfd < 0)
		my_error("Could not establish a socket\n");
	len = sizeof(myaddr);
	bzero(&myaddr, len);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(TCP_PORT);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(tcpfd, (struct sockaddr*)&myaddr, (int)sizeof(myaddr)) < 0)
		my_error("Binding unsuccessfull\n");

	if( listen(tcpfd,10) < 0)
		my_error("Failed to listen\n");
	
	int childfd;
	fromlen = (sizeof(peeraddr));
	
		memset(recvbuf,0,sizeof(recvbuf));
		childfd = accept(tcpfd,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
		
		if(childfd < 0)
			my_error("Failed to accept\n");
		while(1){
			memset(recvbuf,0,sizeof(recvbuf));
			n = recv(childfd,recvbuf,sizeof(recvbuf),0);
			
			if(n<0)
				my_error("Failed to receive\n");
			if(strcmp(recvbuf,"exit") == 0)
				break;
			
			clientdet *a = (clientdet *)malloc(sizeof(clientdet));
			clientlist *alist = (clientlist *)malloc(sizeof(clientlist));

			sscanf(recvbuf,"%s\t%d", a->node_name,&a->link_cost);
			
			alist->data = a;
			load_adjacency(i,a);
			append(alist);
	}
	close(childfd);
	close(tcpfd);
	
}

/*--- UDP Socket ---*/
void create_udp_socket(int port, char *to_send)
{
	int len,udpfd;
	memset(sendbuf,0,sizeof(sendbuf));
	//int fromlen; //int n;
	//struct sockaddr_in myaddr;
	//struct sockaddr_in peeraddr;
	//struct hostent* hp;
	if (to_send != NULL){
		strcpy(sendbuf,to_send);
		usleep(100000);
	}
	else
		strcpy(sendbuf,"send neighbouring data");
		
	//char buf[20];

	udpfd = socket(AF_INET, SOCK_DGRAM,0);
	if(udpfd < 0)
		my_error("Could not establish a socket\n");

	memset(&myaddr,0,sizeof(myaddr));
	
	myaddr.sin_family = SOCK_DGRAM;

	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	myaddr.sin_port = htons(port);
	len = sizeof(struct sockaddr_in);
	
	if(sendto(udpfd, (void *)sendbuf, sizeof(sendbuf),0,(struct sockaddr *) &myaddr,len) < 0)
		my_error("Failed to send\n");
	//char tempbuf[20];	
	
	//recvfrom(udpfd,tempbuf,sizeof(tempbuf),0,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
	/*TCP receive*/
	
	close(udpfd);

}




/*----read from server A ----*/
void read_serverA()
{
	i=0;
	/* UDP query */
	create_udp_socket(UDP_PORT_A,NULL);
	/* TCP response*/
	create_tcp_socket();
	
	
}
	
	

/*---read from server B ---*/
void read_serverB()
{
	i=1;
	/* UDP query */
	create_udp_socket(UDP_PORT_B,NULL);
	
	/*TCP receive*/
	create_tcp_socket();

}
/*---read from server C ---*/
void read_serverC()
{
	i=2;
	/* UDP query */
	create_udp_socket(UDP_PORT_C,NULL);
	
	/*TCP receive*/
	create_tcp_socket();
	
}
/*---read from server D ---*/
void read_serverD()
{
	i=3;
	/* UDP query */
	create_udp_socket(UDP_PORT_D,NULL);
	
	/*TCP receive*/
	create_tcp_socket();
	
	
}
/*Sequentially query the server and receive all the data from the servers*/
void read_from_servers()
{
	read_serverA();
	read_serverB();
	read_serverC();
	read_serverD();
				
}
char adjacencystring_A_1[10];
char adjacencystring_A_2[10];
char adjacencystring_A_3[10];
char adjacencystring_B_1[10];


char adjacencystring_B_2[10];


char adjacencystring_B_3[10];

char adjacencystring_B_1[10];


char adjacencystring_B_2[10];


char adjacencystring_B_3[10];
char adjacencystring_C_1[10];
char adjacencystring_C_2[10];
char adjacencystring_C_3[10];
char adjacencystring_D_1[10];
char adjacencystring_D_2[10];
char adjacencystring_D_3[10];

void send_to_serverA(int i)
{
	memset((adjacencystring_A_1),0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][1] != 0){
		strcat(adjacencystring_A_1,"AB\t");
		sprintf(str1, "%d", adjacency[i][1]);
		strcat(adjacencystring_A_1,str1);
		strcat(adjacencystring_A_1,"\0");
		
	}
	memset(adjacencystring_A_2,0,sizeof(adjacencystring));
	if (adjacency[i][2] != 0)
	{
		strcat(adjacencystring_A_2,"AC\t");
		sprintf(str2, "%d", adjacency[i][2]);
		strcat(adjacencystring_A_2,str2);
		strcat(adjacencystring_A_2,"\0");
		//create_udp_socket(UDP_PORT_A,adjacencystring_A_2);
	}
	memset(adjacencystring_A_3,0,sizeof(adjacencystring));
	if (adjacency[i][3] != 0)
	{
		strcat(adjacencystring_A_3,"AD\t");
		sprintf(str3, "%d", adjacency[i][3]);
		strcat(adjacencystring_A_3,str3);
		strcat(adjacencystring_A_3,"\0");
		//create_udp_socket(UDP_PORT_A,adjacencystring_A_3);
	}
	
		
}
void send_to_serverB(int i)
{
	memset(adjacencystring_B_1,0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][0] != 0){
		strcat(adjacencystring_B_1,"BA\t");
		sprintf(str1, "%d", adjacency[i][0]);
		strcat(adjacencystring_B_1,str1);
		strcat(adjacencystring_B_1,"\0");
		//create_udp_socket(UDP_PORT_B,adjacencystring_B_1);
	}
	memset(adjacencystring_B_2,0,sizeof(adjacencystring));
	if (adjacency[i][2] != 0)
	{
		strcat(adjacencystring_B_2,"BC\t");
		sprintf(str2, "%d", adjacency[i][2]);
		strcat(adjacencystring_B_2,str2);
		strcat(adjacencystring_B_2,"\0");
		//create_udp_socket(UDP_PORT_B,adjacencystring_B_2);
	}
	memset(adjacencystring_B_3,0,sizeof(adjacencystring));
	if (adjacency[i][3] != 0)
	{
		strcat(adjacencystring_B_3,"BD\t");
		sprintf(str3, "%d", adjacency[i][3]);
		strcat(adjacencystring_B_3,str3);
		strcat(adjacencystring_B_3,"\0");
		//create_udp_socket(UDP_PORT_B,adjacencystring_B_3);
	}
	
	
	
	
}
void send_to_serverC(int i)
{
	memset(adjacencystring_C_1,0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][0] != 0){
		strcat(adjacencystring_C_1,"CA\t");
		sprintf(str1, "%d", adjacency[i][0]);
		strcat(adjacencystring_C_1,str1);
		strcat(adjacencystring_C_1,"\0");
		
	}
	memset(adjacencystring_C_2,0,sizeof(adjacencystring));
	if (adjacency[i][1] != 0)
	{
		
		strcat(adjacencystring_C_2,"CB\t");
		sprintf(str2, "%d", adjacency[i][1]);
		strcat(adjacencystring_C_2,str2);
		strcat(adjacencystring_C_2,"\0");
		//create_udp_socket(UDP_PORT_C,adjacencystring);
	}
	memset(adjacencystring_C_3,0,sizeof(adjacencystring));
	if (adjacency[i][3] != 0)
	{
		strcat(adjacencystring_C_3,"CD\t");
		sprintf(str3, "%d", adjacency[i][3]);
		strcat(adjacencystring_C_3,str3);
		strcat(adjacencystring_C_3,"\0");
		//create_udp_socket(UDP_PORT_C,adjacencystring_C_3);
	}
	//create_udp_socket(UDP_PORT_C,adjacencystring);
	
	
}
void send_to_serverD(int i)
{
	memset(adjacencystring_D_1,0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][0] != 0){
		strcat(adjacencystring_D_1,"DA\t");
		sprintf(str1, "%d", adjacency[i][0]);
		strcat(adjacencystring_D_1,str1);
		strcat(adjacencystring_D_1,"\0");
		//create_udp_socket(UDP_PORT_C,adjacencystring);
	}
	memset(adjacencystring_D_2,0,sizeof(adjacencystring));
	if (adjacency[i][1] != 0)
	{
		strcat(adjacencystring_D_2,"DB\t");
		sprintf(str2, "%d", adjacency[i][1]);
		strcat(adjacencystring_D_2,str2);
		strcat(adjacencystring_D_2,"\0");
		//create_udp_socket(UDP_PORT_C,adjacencystring);
	}
	memset(adjacencystring_D_3,0,sizeof(adjacencystring));
	if (adjacency[i][2] != 0)
	{
		strcat(adjacencystring_D_3,"DC\t");
		sprintf(str3, "%d", adjacency[i][2]);
		strcat(adjacencystring_D_3,str3);
		strcat(adjacencystring_D_3,"\0");
		
	}
	//create_udp_socket(UDP_PORT_D,adjacencystring);
	
	
}

void broadcast()
{
	send_to_serverA(0);
	send_to_serverB(1);
	send_to_serverC(2);
	send_to_serverD(3);
}
void send_port(int port)
{
	if (strlen(adjacencystring_A_1) != 0)
		create_udp_socket(port,adjacencystring_A_1);
	if (strlen(adjacencystring_A_2) != 0)
		create_udp_socket(port,adjacencystring_A_2);
	if (strlen(adjacencystring_A_3) != 0)
		create_udp_socket(port,adjacencystring_A_3);
	if (strlen(adjacencystring_B_1) != 0)
		create_udp_socket(port,adjacencystring_B_1);
	if (strlen(adjacencystring_B_2) != 0)
		create_udp_socket(port,adjacencystring_B_2);
	if (strlen(adjacencystring_B_3) != 0)
		create_udp_socket(port,adjacencystring_B_3);
	if (strlen(adjacencystring_C_1) != 0)
		create_udp_socket(port,adjacencystring_C_1);
	if (strlen(adjacencystring_C_2) != 0)
		create_udp_socket(port,adjacencystring_C_2);
	if (strlen(adjacencystring_C_3) != 0)
		create_udp_socket(port,adjacencystring_C_3);
	if (strlen(adjacencystring_D_1) != 0)
		create_udp_socket(port,adjacencystring_D_1);
	if (strlen(adjacencystring_D_2) != 0)
		create_udp_socket(port,adjacencystring_D_2);
	if (strlen(adjacencystring_D_3) != 0)
		create_udp_socket(port,adjacencystring_D_3);
	create_udp_socket(port,"exit");
}

int main()
{
	/*--- Read the details from the given ---*/
	head.next = NULL;
	head.data = NULL;
	memset(adjacency,0,sizeof(adjacency));
	
	read_from_servers();
	/*Flatten the list*/
	broadcast();
	
	send_port(UDP_PORT_A);
	send_port(UDP_PORT_B);
	send_port(UDP_PORT_C);
	send_port(UDP_PORT_D);
	
	
	
	
	return 0;
}
