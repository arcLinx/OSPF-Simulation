/*#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
pthread_t thread[2];
*/
/*---- Create a passive UDP socket for the Server A----*/


/*Global variables for creating the socket
typedef struct address{
	int serverAIP;
	int serverAPort;
}my_address;
*/

/*---- Thread for reading the file of the server when it is boot up ---
void *serverBoot(void *arg)
{
	// Read the file Servera.txt
	return NULL;
}
*/
/*----- Thread for establishing a connection*
void *serverConnect(void *arg)
{
	//Establish a UDP socket
	int sd;
	char buf[1024];
	sd = socket(AF_INET,SOCK_DGRAM,0); // The domain arguement specifies the family of the communiation used and type argument tells which protocol to use (TCP/ UDP) and protocol arguement tells that this 										particular socket uses which differenent protocol types
	my_address* addr = NULL;
	addr->serverAIP = 1234;
	addr->serverAPort = 90;
	
	//Address of the client
	my_address *client_addr = malloc(sizeof(my_address));
	int client_addrlen =  0;

	//Bind the socket descriptor
	if( bind(sd, addr,sizeof(my_address)) == -1)
		printf("Failed to bind!!\n");
	recvfrom(sd,buf,1,client_addrlen);
	send(sd,buf,0);
	return NULL;
}

void *serverSend(void *arg)
{
	connect();
	send();

	return NULL;
}
int main()
{
	
*/	
	/*pthread_create(&thread[0], NULL, serverBoot, NULL);*
	pthread_create(&thread[1], NULL, serverConnect, NULL);
	pthread_create(&thread[2], NULL, serverSend,NULL);


	
	//pthread_join(thread[0]);
	pthread_join(thread[1],NULL);
	//pthread_join(thread[2]);
	
	return 0;
}
*/
#include<stdio.h>
#include<stdlib.h>
/*--- Data structure for storing the neighbouring information ----*/
typedef struct server{
	struct server *next;
	struct server *prev;
	void *data;
}serverA;

/*--- Data structure for storing the details ----*/

typedef struct srvA{
	char node_name[10];
	int link_cost;
}srvrAdat;

/*---- Append the node to the linked list ----*/
void append()
{
	// to head
	// to other node
	
}

/*---- Read the file ServerA.txt as it is ----*/

void serverBootUp()
{
	char buf[1024];
	FILE *fp = NULL;
	fp = fopen("serverA.txt","r");
	
			
	while(fgets(buf,sizeof(buf),fp) != NULL)
	{
		serverA *a = (serverA *)malloc(sizeof(serverA));
		srvrAdat *adat = (srvrAdat *)malloc(sizeof(serverA));
		
 		sscanf(buf,"%s\t%d", adat->node_name,&adat->link_cost);
 		printf("%s \t %d \n",adat->node_name, adat->link_cost);
 		a->data = adat;
 		
 		append(a);
		
	}
}



int main()
{
	
	serverBootUp();
	return 0;
}
