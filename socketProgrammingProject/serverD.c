#include<stdio.h>
#include<stdlib.h>
/*--- Data structure for storing the neighbouring information ----*/
typedef struct server{
	struct server *next;
	struct server *prev;
	void *data;
}serverD;

/*--- Data structure for storing the details ----*/

typedef struct srvD{
	char node_name[10];
	int link_cost;
}srvrDdat;

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
	fp = fopen("serverD.txt","r");
	
			
	while(fgets(buf,sizeof(buf),fp) != NULL)
	{
		serverD *d = (serverD *)malloc(sizeof(serverD));
		srvrDdat *ddat = (srvrDdat *)malloc(sizeof(serverD));
		
 		sscanf(buf,"%s\t%d", ddat->node_name,&ddat->link_cost);
 		printf("%s \t %d \n",ddat->node_name, ddat->link_cost);
 		d->data = ddat;
 		
 		append(d);
		
	}
}



int main()
{
	
	serverBootUp();
	return 0;
}
