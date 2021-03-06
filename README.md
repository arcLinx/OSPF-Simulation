##########################################
##    Socket Programming Assignment     ##
##  			README-File      	            ##
##      Author: ARCHISH YOGA RANGAN     ##
##########################################

1.  In this assignment I have simulated a server-client model (using TCP and UDP sockets), that exchanges information about the neighboring nodes and learns the shortest
	path to reach any node on the given network. The program execution flow is as follows: All the servers block on the UDP recvfrom call, and the client queries for the
	neighbor information with each server, immediately the client starts a TCP socket and blocks on the accept call and the server creates a TCP socket to connect to the
	client and neighbor information is passed. And the client broadcasts the network topology through a UDP socket. 

2. The code files used in this program are as follows: All the servers have the neighbor cost information when they bootup.
	a. serverA.c- Listens on UDP socket for the client, once the client asks for the neighbors data it starts a TCP socket and sends the Neighbor cost data to the client
	through and closes the TCP connection. Then, blocks on the UDP socket to receive the entire network topology and prints it.

	b. serverB.c- Listens on UDP socket for the client, once the client asks for the neighbors data it starts a TCP socket and sends the Neighbor cost data to the client
	through and closes the TCP connection. Then, blocks on the UDP socket to receive the entire network topology and prints it.

	c. serverC.c- Listens on UDP socket for the client, once the client asks for the neighbors data it starts a TCP socket and sends the Neighbor cost data to the client
	through and closes the TCP connection. Then, blocks on the UDP socket to receive the entire network topology and prints it.

	d. serverD.c- Listens on UDP socket for the client, once the client asks for the neighbors data it starts a TCP socket and sends the Neighbor cost data to the client
	through and closes the TCP connection. Then, blocks on the UDP socket to receive the entire network topology and prints it.

	e. client.c- Connects to the server A on UDP and blocks on the TCP accept call, receives the Neighbor data from server and closes the TCP connection and prints the 
	received information, and this process is continued with all servers sequentially(server B ,server C, server D). And the client computes the network topology and 
	sends to the each server (server A , server B, server C, server D are blocked on recvfrom call) and prints them. Finally, it computes the minimum spanning tree from the 
	network topology and prints the same.

3. Please use the following commands to run the program. This program requires the following order of execution to work.
	a. use "make" command to create the executable for all the .c files present.
	b. Fire server A in terminal 1 by using the command ./serverA
	c. Fire server B in terminal 2 by using the command ./serverB
	d. Fire server C in terminal 3 by using the command ./serverC
	e. Fire server D in terminal 4 by using the command ./serverD
	f. Fire the client in terminal 5 by using the command ./client 

4. All the messages from the server and client are through strings (character arrays).

5. No idiosyncrasy in the program.

6. Minimum spanning Tree algorithm and Linux Programmer's manual (For socket structures and system calls) were referred.


+++++++++++++++
| Other Notes |
+++++++++++++++

1. This program is developed using C programming language on Linux Machine.

2. File I/O operations are used to for inter process communication. However, all the files are closed and deleted when the program terminates.

3. This program uses usleep to sync the server and the client communication process. Hence, the output takes time to print. However, the program terminates cleanly.
