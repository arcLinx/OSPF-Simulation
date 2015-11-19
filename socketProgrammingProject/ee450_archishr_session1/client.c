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
int server_tcp_dynamic_port;


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

/*---- Peer address and IP address ----*/
char peer_ip_address[20];
int peer_port;

/*Global String variables to be used*/
int flag_a1 = 0, flag_a2 = 0, flag_a3 = 0, flag_b1 =0 , flag_b2 =0 ,flag_c1 = 0;
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


/*---- host address and IP address ----*/
struct in_addr **host_addr;
char host_ip[100];


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


/*---- TCP Socket creation for accepting neighbor cost information from all servers---*/
void create_tcp_socket(char *server_name)
{
	int fromlen,tcpfd, n,len;
	int yes = 1;
	
	len = sizeof(myaddr);
	bzero(&myaddr, len);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(TCP_PORT);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	/*---- Create the TCP socket descriptor----*/
	tcpfd = socket(AF_INET,SOCK_STREAM,0);
	if(tcpfd < 0)
		my_error("Could not establish a socket\n");
		
	setsockopt(tcpfd,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	
	/*---- Bind the client to the  static TCP port number and localhost IP address of the created TCP socket descriptor----*/
	if(bind(tcpfd, (struct sockaddr*)&myaddr, (int)sizeof(myaddr)) < 0)
		my_error("Binding unsuccessfull\n");
	/*---- Listen on the address ----*/
	if( listen(tcpfd,10) < 0)
		my_error("Failed to listen\n");
	
	int childfd;
	fromlen = (sizeof(peeraddr));
		/*---- Block on accept call to establish the connection with servers----*/
		memset(recvbuf,0,sizeof(recvbuf));
		childfd = accept(tcpfd,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
		
		if(childfd < 0)
			my_error("Failed to accept\n");
		
		/*---- Get the peer address to be used ----*/
		getpeername(childfd, (struct sockaddr*)&peeraddr,(socklen_t *) &fromlen);
		struct sockaddr_in *s = (struct sockaddr_in *)&peeraddr;
		server_tcp_dynamic_port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, peer_ip_address, sizeof peer_ip_address);
		
		printf("The Client receives neighbor information from the %s with TCP port number %d and IP address %s\n",server_name,server_tcp_dynamic_port,peer_ip_address);
		printf("The %s has the following neighbor information:\n",server_name);
		printf("Neighbor------Cost\n");
		/*---- Iteratively get the neighbour and cost from the servers- and print them---*/
		while(1){
			memset(recvbuf,0,sizeof(recvbuf));
			/*--- Block on the recevive call to get data from the servers*/
			n = recv(childfd,recvbuf,sizeof(recvbuf),0);
			
			if(n<0)
				my_error("Failed to receive\n");
			if(strcmp(recvbuf,"exit") == 0)
				break;
			
			clientdet *a = (clientdet *)malloc(sizeof(clientdet));
			clientlist *alist = (clientlist *)malloc(sizeof(clientlist));

			sscanf(recvbuf,"%s\t%d", a->node_name,&a->link_cost);
			printf("%s       %d\n", a->node_name,a->link_cost);
			alist->data = a;
			load_adjacency(i,a);
			append(alist);
	}
	close(childfd);
	close(tcpfd);
	
}

/*--- UDP Socket creation for exchanging dta  ---*/
void create_udp_socket(int port, char *to_send)
{
	int len,udpfd;
	memset(sendbuf,0,sizeof(sendbuf));
	
	struct hostent* hp;
	if (to_send != NULL){
		strcpy(sendbuf,to_send);
		/*----This is for syncing the server and client communication ----*/
		usleep(1000000);
	}
	else
		strcpy(sendbuf,"send neighbouring data");
		
	udpfd = socket(AF_INET, SOCK_DGRAM,0);
	if(udpfd < 0)
		my_error("Could not establish a socket\n");
		
	/*----Local Host IP address and port number resolve ----*/
	 if ( (hp = gethostbyname( "localhost" ) ) == NULL) 
		my_error("gethostbyname");
    
     host_addr = (struct in_addr **) hp->h_addr_list;
    int i;
    for(i = 0; host_addr[i] != NULL; i++) 
		strcpy(host_ip , inet_ntoa(*host_addr[i]) );

	memset(&myaddr,0,sizeof(myaddr));
	
	myaddr.sin_family = AF_INET;

	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	myaddr.sin_port = htons(port);
	len = sizeof(struct sockaddr_in);
	/*----Send the data to the servers initial query and the network topology ----*/
	if(sendto(udpfd, (void *)sendbuf, sizeof(sendbuf),0,(struct sockaddr *) &myaddr,len) < 0)
		my_error("Failed to send\n");
	
	close(udpfd);
	

}




/*----read from server A ----*/
void read_serverA()
{
	i=0;
	/* UDP query */
	create_udp_socket(UDP_PORT_A,NULL);
	/* TCP response*/
	create_tcp_socket("Server A");
	printf("For this connection with Server A, The Client has TCP port number %d and IP address %s\n",TCP_PORT,host_ip);
}
	
	

/*---read from server B ---*/
void read_serverB()
{
	i=1;
	/*UDP query*/
	create_udp_socket(UDP_PORT_B,NULL);
	/*TCP receive*/
	create_tcp_socket("Server B");
	printf("For this connection with Server B, The Client has TCP port number %d and IP address %s\n",TCP_PORT,host_ip);

}
/*---read from server C ---*/
void read_serverC()
{
	i=2;
	/*UDP query*/
	create_udp_socket(UDP_PORT_C,NULL);
	/*TCP receive*/
	create_tcp_socket("Server C");
	printf("For this connection with Server C, The Client has TCP port number %d and IP address %s\n",TCP_PORT,host_ip);
}
/*---read from server D ---*/
void read_serverD()
{
	i=3;
	/*UDP query*/
	create_udp_socket(UDP_PORT_D,NULL);
	/*TCP receive*/
	create_tcp_socket("Server D");
	printf("For this connection with Server D, The Client has TCP port number %d and IP address %s\n",TCP_PORT,host_ip);
}
/*Sequentially query the server and receive all the data from the servers*/
void read_from_servers()
{
	read_serverA();
	read_serverB();
	read_serverC();
	read_serverD();			
}

/* Load the adjacency string to send the matrix to Server A*/
void send_to_serverA(int i)
{
	memset((adjacencystring_A_1),0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][1] != 0){
		strcat(adjacencystring_A_1,"AB\t");
		sprintf(str1, "%d", adjacency[i][1]);
		strcat(adjacencystring_A_1,str1);
		strcat(adjacencystring_A_1,"\0");
		flag_a1 =1;
	}
	memset(adjacencystring_A_2,0,sizeof(adjacencystring));
	if (adjacency[i][2] != 0)
	{
		strcat(adjacencystring_A_2,"AC\t");
		sprintf(str2, "%d", adjacency[i][2]);
		strcat(adjacencystring_A_2,str2);
		strcat(adjacencystring_A_2,"\0");
		flag_a2 =1;
	}
	memset(adjacencystring_A_3,0,sizeof(adjacencystring));
	if (adjacency[i][3] != 0)
	{
		strcat(adjacencystring_A_3,"AD\t");
		sprintf(str3, "%d", adjacency[i][3]);
		strcat(adjacencystring_A_3,str3);
		strcat(adjacencystring_A_3,"\0");
		flag_a3 =1;
	}
	
		
}

/* Load the adjacency string to send the matrix to Server B*/
void send_to_serverB(int i)
{
	memset(adjacencystring_B_1,0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][0] != 0){
		if(flag_a1 == 0){
			strcat(adjacencystring_B_1,"BA\t");
			sprintf(str1, "%d", adjacency[i][0]);
			strcat(adjacencystring_B_1,str1);
			strcat(adjacencystring_B_1,"\0");
		}
	}
	memset(adjacencystring_B_2,0,sizeof(adjacencystring));
	if (adjacency[i][2] != 0)
	{
		strcat(adjacencystring_B_2,"BC\t");
		sprintf(str2, "%d", adjacency[i][2]);
		strcat(adjacencystring_B_2,str2);
		strcat(adjacencystring_B_2,"\0");
		flag_b1 =1;
	}
	memset(adjacencystring_B_3,0,sizeof(adjacencystring));
	if (adjacency[i][3] != 0)
	{
		strcat(adjacencystring_B_3,"BD\t");
		sprintf(str3, "%d", adjacency[i][3]);
		strcat(adjacencystring_B_3,str3);
		strcat(adjacencystring_B_3,"\0");
		flag_b2 = 1;
	}
}
/* Load the adjacency string to send the matrix to Server C*/
void send_to_serverC(int i)
{
	memset(adjacencystring_C_1,0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][0] != 0){
		if(flag_a2 == 0){
		strcat(adjacencystring_C_1,"CA\t");
		sprintf(str1, "%d", adjacency[i][0]);
		strcat(adjacencystring_C_1,str1);
		strcat(adjacencystring_C_1,"\0");
		}
	}
	memset(adjacencystring_C_2,0,sizeof(adjacencystring));
	if (adjacency[i][1] != 0)
	{
		if(flag_b1 == 0){
		strcat(adjacencystring_C_2,"CB\t");
		sprintf(str2, "%d", adjacency[i][1]);
		strcat(adjacencystring_C_2,str2);
		strcat(adjacencystring_C_2,"\0");
		}
	}
	memset(adjacencystring_C_3,0,sizeof(adjacencystring));
	if (adjacency[i][3] != 0)
	{
		strcat(adjacencystring_C_3,"CD\t");
		sprintf(str3, "%d", adjacency[i][3]);
		strcat(adjacencystring_C_3,str3);
		strcat(adjacencystring_C_3,"\0");
		flag_c1 =1;
	}
}
/* Load the adjacency string to send the matrix to Server D*/
void send_to_serverD(int i)
{
	memset(adjacencystring_D_1,0,sizeof(adjacencystring));
	char str1[10],str2[10],str3[10];
	if (adjacency[i][0] != 0){
		if(flag_a3 == 0){
		strcat(adjacencystring_D_1,"DA\t");
		sprintf(str1, "%d", adjacency[i][0]);
		strcat(adjacencystring_D_1,str1);
		strcat(adjacencystring_D_1,"\0");
	}
	}
	memset(adjacencystring_D_2,0,sizeof(adjacencystring));
	if (adjacency[i][1] != 0)
	{
		if(flag_b2 == 0){
		strcat(adjacencystring_D_2,"DB\t");
		sprintf(str2, "%d", adjacency[i][1]);
		strcat(adjacencystring_D_2,str2);
		strcat(adjacencystring_D_2,"\0");
	}
	}
	memset(adjacencystring_D_3,0,sizeof(adjacencystring));
	if (adjacency[i][2] != 0)
	{
		if(flag_c1 == 0){
		strcat(adjacencystring_D_3,"DC\t");
		sprintf(str3, "%d", adjacency[i][2]);
		strcat(adjacencystring_D_3,str3);
		strcat(adjacencystring_D_3,"\0");
	}
	}	
	
}
/*--- This function loads the adjacency string specific to the servers sequentially ----*/

void broadcast()
{
	send_to_serverA(0);
	send_to_serverB(1);
	send_to_serverC(2);
	send_to_serverD(3);
}
char buf_A[20],buf_B[20],buf_C[20],buf_D[20];
/*---- This function creates UDP socket to send the network Topology to all the servers ----*/
void send_port(int port, char* file_name)
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
	FILE *fp=NULL;
	fp = fopen("UDP_port.txt","r");
	if (strcmp(file_name,"Server A")==0)
		fgets(buf_A,sizeof(buf_A),fp);
	if (strcmp(file_name,"Server B")==0)
		fgets(buf_B,sizeof(buf_B),fp);
	if (strcmp(file_name,"Server C")==0)
		fgets(buf_C,sizeof(buf_C),fp);
	if (strcmp(file_name,"Server D")==0)
		fgets(buf_D,sizeof(buf_D),fp);
	fclose(fp);
	
	create_udp_socket(port,"exit");
}

/*----Function to print the network Topology ----*/
void print_net_top(char *server_name,int port,int c_port)
{
	
	printf("The Client has sent the network topology to the %s with UDP port number %d and IP address %s\n",server_name,port,peer_ip_address);
	printf("Edge----Cost\n");
	if (strlen(adjacencystring_A_1) != 0)
		printf("%s\n",adjacencystring_A_1);
	if (strlen(adjacencystring_A_2) != 0)
		printf("%s\n",adjacencystring_A_2);
	if (strlen(adjacencystring_A_3) != 0)
		printf("%s\n",adjacencystring_A_3);
	if (strlen(adjacencystring_B_1) != 0)
		printf("%s\n",adjacencystring_B_1);
	if (strlen(adjacencystring_B_2) != 0)
		printf("%s\n",adjacencystring_B_2);
	if (strlen(adjacencystring_B_3) != 0)
		printf("%s\n",adjacencystring_B_3);
	if (strlen(adjacencystring_C_1) != 0)
		printf("%s\n",adjacencystring_C_1);
	if (strlen(adjacencystring_C_2) != 0)
		printf("%s\n",adjacencystring_C_2);
	if (strlen(adjacencystring_C_3) != 0)
		printf("%s\n",adjacencystring_C_3);
	if (strlen(adjacencystring_D_1) != 0)
		printf("%s\n",adjacencystring_D_1);
	if (strlen(adjacencystring_D_2) != 0)
		printf("%s\n",adjacencystring_D_2);
	if (strlen(adjacencystring_D_3) != 0)
		printf("%s\n",adjacencystring_D_3);
	printf("For this connection with %s, The Client has UDP port number %d and IP address %s\n",server_name,c_port,peer_ip_address);
}

/*---- Function to compute and print the minimum spanning tree, This code logic was taken from one of the opensource website----*/
void min_spanning_tree()
{
	int a,b,u,v,i,j,ne=1;
    int visited[10]={0},min,min_cost=0;
    char min_tree[1024];
    memset(min_tree,0,sizeof(min_tree));
    char cost[20];
    visited[0]=1;
    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(i!=j && adjacency[i][j] == 0) {
                adjacency[i][j]=999;
            }
        }
    }
    while(ne < 4)
    {
        for(i=0,min=999;i<4;i++)
            for(j=0;j<4;j++)
                if(adjacency[i][j]< min)
                    if(visited[i]!=0)
                    {
                        min=adjacency[i][j];
                        a=u=i;
                        b=v=j;
                    }
        
        if(visited[u]==0 || visited[v]==0)
        {
            if(a==0)
            {
                if(b == 1) {
                    strcat(min_tree,"AB\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
                if (b == 2) {
                    strcat(min_tree,"AC\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
                if(b == 3) {
                    strcat(min_tree,"AD\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
            }
            else if(a==1)
            {
                if(b == 0) {
                    strcat(min_tree,"BA\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
                if(b == 2) {
                    strcat(min_tree,"BC\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");    
                }
                if(b == 3) {
                    strcat(min_tree,"BD\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
            }
            else if(a==2)
            {
                if(b == 0) {
                    strcat(min_tree,"CA\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");    
                }
                if(b == 1) {
                    strcat(min_tree,"CB\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
                if(b == 3) {
                    strcat(min_tree,"CD\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
            }
            else if(a==3)
            {
                if(b == 0) {
                    strcat(min_tree,"DA\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
                if(b == 1) {
                    strcat(min_tree,"DB\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
                if(b == 2) {
                    strcat(min_tree,"DC\t");
                    sprintf(cost,"%d",min);
                    strcat(min_tree,cost);
                    strcat(min_tree,"\n");
                }
            }
            min_cost+=min;
            visited[b]=1;
            ne++;
        }
        adjacency[a][b]=adjacency[b][a]=999;
    }
    printf("The Client has calculated a tree.The tree cost is %d\n",min_cost);
    printf("Edge------Cost\n");
    printf("%s",min_tree);
}

int main()
{
	/*--- Initialize the head of the linked list ---*/
	head.next = NULL;
	head.data = NULL;
	memset(adjacency,0,sizeof(adjacency));
	struct hostent* hp;
	
	/*----Local Host IP address and port number resolve ----*/
	 if ( (hp = gethostbyname( "localhost" ) ) == NULL) 
		my_error("gethostbyname Failed \n");
    
     host_addr = (struct in_addr **) hp->h_addr_list;
    int i;
    for(i = 0; host_addr[i] != NULL; i++)
		strcpy(host_ip , inet_ntoa(*host_addr[i]) );
	/*----Onscreen Messages ----*/
	printf("The Client has TCP port number %d and IP address %s\n",TCP_PORT,host_ip);
	
	/*---- Sequentially Query all the servers on UDP socket and receive the data on TCP socket and load the linked list----*/
	read_from_servers();
	
	/*---- Compute the network Topology cummulatively after receving the neighbor cost information from all the servers ----*/
	broadcast();
	/*---- Send the Adjacency Matrix, the network topology to the server A ---*/
	send_port(UDP_PORT_A,"Server A");
	/*---- Send the Adjacency Matrix, the network topology to the server B ---*/
	send_port(UDP_PORT_B,"Server B");
	/*---- Send the Adjacency Matrix, the network topology to the server C ---*/
	send_port(UDP_PORT_C,"Server C");
	/*---- Send the Adjacency Matrix, the network topology to the server D ---*/
	send_port(UDP_PORT_D,"Server D");
	/*---- Print the Network topology sent to the server A----*/
	print_net_top("Server A",UDP_PORT_A,atoi(buf_A));
	/*---- Print the Network topology sent to the server B----*/
	print_net_top("Server B",UDP_PORT_B,atoi(buf_B));
	/*---- Print the Network topology sent to the server C----*/
	print_net_top("Server C",UDP_PORT_C,atoi(buf_C));
	/*---- Print the Network topology sent to the server D----*/
	print_net_top("Server D",UDP_PORT_D,atoi(buf_D));
	
	/*---- Compute the Minimum spanning tree cost and print the minimum spanning tree -----*/
	min_spanning_tree();
	remove("UDP_port.txt");
	return 0;
}
