serverA: serverA.o serverB serverC serverD client
	gcc -o serverA -g serverA.o -lsocket -lnsl -lresolv

serverA.o: serverA.c
	gcc -g -c -Wall serverA.c -lsocket -lnsl -lresolv

serverB: serverB.o 
	gcc -o serverB -g serverB.o -lsocket -lnsl -lresolv

serverB.o: serverB.c
	gcc -g -c -Wall serverB.c -lsocket -lnsl -lresolv
	
serverC: serverC.o 
	gcc -o serverC -g serverC.o -lsocket -lnsl -lresolv

serverC.o: serverC.c
	gcc -g -c -Wall serverC.c -lsocket -lnsl -lresolv

serverD: serverD.o 
	gcc -o serverD -g serverD.o -lsocket -lnsl -lresolv

serverD.o: serverD.c
	gcc -g -c -Wall serverD.c -lsocket -lnsl -lresolv
	
client: client.o 
	gcc -o client -g client.o -lsocket -lnsl -lresolv

client.o: client.c
	gcc -g -c -Wall client.c -lsocket -lnsl -lresolv
clean:
	rm -f *.o serverA serverB serverC serverD client
