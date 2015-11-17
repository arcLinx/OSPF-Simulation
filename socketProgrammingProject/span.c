#include <stdio.h>
#include <string.h>

int main()
{
	int a,b,u,v,i,j,ne=1;
	int visited[10]={0},min,mincost=0,cost[10][10];
	
	visited[1]=1;
	printf("\n");
	while(ne < 4)
	{
		for(i=1,min=999;i<=4;i++)
		for(j=1;j<=4;j++)
		if(cost[i][j]< min)
		if(visited[i]!=0)
		{
			min=cost[i][j];
			a=u=i;
			b=v=j;
		}
		if(visited[u]==0 || visited[v]==0)
		{
			if(a==1)
			{
				if(b == 2)
					printf("AB\t%d\n",min);
				if (b == 3)
					printf("AC\t%d\n",min);
				if(b == 4)
					printf("AD\t%d\n",min);
			}
			else if(a==2)
			{
				if(b == 1)
					printf("BA\t%d\n",min);
				if(b == 3)
					printf("BC\t%d\n",min);
				if(b == 4) 
					printf("BD\t%d\n",min);
			}
			else if(a==3)
			{
				if(b == 1)
					printf("CA\t%d\n",min);
				if(b == 2)
					printf("CB\t%d\n",min);
				if(b == 4)
					printf("CD\t%d\n",min);
			}
			else if(a==4)
			{
				if(b == 1)
					printf("DA\t%d\n",min);
				if(b == 2)
					printf("DB\t%d\n",min);
				if(b == 3) 
					printf("DC\t%d\n",min);
			}
			printf("\n Edge %d:(%d %d) cost:%d",ne++,a,b,min);
			mincost+=min;
			visited[b]=1;
		}
		cost[a][b]=cost[b][a]=999;
	}
	printf("\n Minimun cost=%d",mincost);
	//getc();
	return 0;
}
