#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define UDP_PORT 1234
void my_error(char *msg)
{
	perror(msg);
	exit(1);
}
typedef struct pass{
	char name[10];
	int age;
}pass1;

int main(int argc, char* argv[])

{
	int sockfd, fromlen, n,len;
	struct sockaddr_in myaddr;
	struct sockaddr_in peeraddr;
	//struct hostent *hp;
	char buf[1024],temp[10],buf1[1024];
	pass1 p;
	p.age=24;
	strcpy(p.name,"Geets");
	sprintf(temp,"%d", p.age);
	strcat(buf1,p.name);
	strcat(buf1,"\t");
	strcat(buf1,temp);
		
	sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
		my_error("Could not establish a socket\n");


	len = sizeof(myaddr);
	bzero(&myaddr, len);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(UDP_PORT);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	if( bind(sockfd, (struct sockaddr*)&myaddr, (int)sizeof(myaddr)) < 0)
		my_error("Binding unsuccessfull\n");
	
	
	fromlen = (sizeof(peeraddr));
	
	n = recvfrom(sockfd,(void *)buf,sizeof(buf),0,(struct sockaddr*)&peeraddr,(socklen_t *)&fromlen);
	if(n<0)
		my_error("Failed to receive\n");
	
	printf("%s\n",buf);
	sendto(sockfd,buf1, sizeof(buf1),0,(struct sockaddr *) &peeraddr,fromlen);

	

	close(sockfd);
	return 0;
}
	


	
