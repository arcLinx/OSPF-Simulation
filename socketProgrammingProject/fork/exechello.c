#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main ()
{
	pid_t c = fork();
	if(c ==0)
		execl("/home/archie/EE450/socketProgrammingProject/helloworld","helloworld",NULL,(char*) 0);
	int status;
	wait(&status);
	return 0;
}
