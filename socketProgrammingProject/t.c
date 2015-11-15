#include<stdio.h>
#include<string.h>

int main()
{
	int aInt = 368;
	char str[150]="geets\tasdf";
	char a[10];
	sprintf(a, "%d", aInt);	
//	strcat(str,a);
	char test[101],test1[20];
	sscanf(str,"%s\t%s",test,test1);
	printf("%s\n%s\n",test,test1);
	return 0;
}
