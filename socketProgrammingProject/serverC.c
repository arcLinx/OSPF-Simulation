#include<stdio.h>
#include<stdlib.h>
/*--- Data structure for storing the neighbouring information ----*/
typedef struct server{
	struct server *next;
	struct server *prev;
	void *data;
}serverC;

/*--- Data structure for storing the details ----*/

typedef struct srvC{
	char node_name[10];
	int link_cost;
}srvrCdat;

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
	fp = fopen("serverC.txt","r");
	
			
	while(fgets(buf,sizeof(buf),fp) != NULL)
	{
		serverC *c = (serverC *)malloc(sizeof(serverC));
		srvrCdat *cdat = (srvrCdat *)malloc(sizeof(serverC));
		
 		sscanf(buf,"%s\t%d", cdat->node_name,&cdat->link_cost);
 		printf("%s \t %d \n",cdat->node_name, cdat->link_cost);
 		c->data = cdat;
 		
 		append(c);
		
	}
}



int main()
{
	
	serverBootUp();
	return 0;
}
