#include<stdio.h>
#include<stdlib.h>
/*--- Data structure for storing the neighbouring information ----*/
typedef struct server{
	struct server *next;
	struct server *prev;
	void *data;
}serverB;

/*--- Data structure for storing the details ----*/

typedef struct srvB{
	char node_name[10];
	int link_cost;
}srvrBdat;

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
	fp = fopen("serverB.txt","r");
	
			
	while(fgets(buf,sizeof(buf),fp) != NULL)
	{
		serverB *b = (serverB *)malloc(sizeof(serverB));
		srvrBdat *bdat = (srvrBdat *)malloc(sizeof(serverB));
		
 		sscanf(buf,"%s\t%d", bdat->node_name,&bdat->link_cost);
 		printf("%s \t %d \n",bdat->node_name, bdat->link_cost);
 		b->data = bdat;
 		
 		append(b);
		
	}
}



int main()
{
	
	serverBootUp();
	return 0;
}
