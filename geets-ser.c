/*EE450 - Client-Server Program */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>

#define SERVER_TCP_PORT 25853
#define SERVER_A_UDP_PORT 31853

typedef struct serverAList {
    void *obj;
    struct serverAList *next;
} MyserverAList;

typedef struct serverA{
    char host_name[50];
    int  cost_link;
}serverA_t;

MyserverAList *anchor = NULL;
int flag = 1, udp_serv_fd = 0, tcp_sfd = 0;
struct sockaddr_in server_udp;
struct sockaddr_in server_tcp;

void error(char *message) {
    perror(message);
    exit(1);
}

void display_list() {
    MyserverAList *node = anchor; 
    while(node->next != NULL) {
        node = node->next;
        printf("%s\t",((serverA_t *)node->obj)->host_name);
        printf("%d\n",((serverA_t *)node->obj)->cost_link);
    }
}

void insert(MyserverAList *myList,serverA_t *serA) {
    MyserverAList *f = anchor;
    if(flag) {
        myList->next = NULL;
        myList->obj = (void *)serA;
        f->next = myList;
        flag = 0;
    } else {
        while(f->next != NULL) {
            f = f->next;
        }
        myList->next = NULL;
        myList->obj = (void *)serA;
        f->next = myList;
    }
}

void read_input_file(FILE *handler) {
    char buf[1024];
    MyserverAList *myList = NULL;
    serverA_t *serA = NULL; 
    while(fgets(buf,sizeof(buf),handler)) {
        myList = (MyserverAList *)malloc(sizeof(MyserverAList));
        serA = (serverA_t *)malloc(sizeof(serverA_t));
        sscanf(buf,"%s\t%d",serA->host_name,&serA->cost_link);
        
        insert(myList,serA);
    }
}

/* Create server UDP socket */
void create_socket_udp(int flag) {
    
    int bytes_received = 0;
    socklen_t length = sizeof(struct sockaddr_in);
    char receive_data[1024],adjacency[100];
    int cost;
    
    if((udp_serv_fd = socket(AF_INET,SOCK_DGRAM,0))==-1) {
                error("UDP socket for serverA");
    }
    server_udp.sin_family = AF_INET;
    server_udp.sin_port = htons(SERVER_A_UDP_PORT);
    server_udp.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_udp.sin_zero),8);
    
    if(bind(udp_serv_fd,(struct sockaddr *)&server_udp,(int)length) < 0) {
            error("server: bind");
    }
    if(flag == 0) {
        if ((bytes_received = recvfrom(udp_serv_fd, receive_data, sizeof(receive_data), 0,(struct sockaddr *)&server_udp,&length)) == -1) {
            error("serverA: recvfrom");
        }
        printf("server received data from client: %s\n",receive_data);
    } else {
        while(1) {
        memset(receive_data,0,sizeof(receive_data));
        if ((bytes_received = recvfrom(udp_serv_fd, receive_data, sizeof(receive_data), 0,(struct sockaddr *)&server_udp,&length)) == -1) {
            error("serverA: recvfrom"); 
        }
            if(strcmp(receive_data,"exit") == 0) 
                break;
            sscanf(receive_data,"%s\t%d",adjacency,&cost);
            printf("Edge: %s\tCost: %d\n",adjacency,cost);        
        }
    }
    close(udp_serv_fd);
}

/* Create server TCP socket */
void create_socket_tcp() {
    int bytes_sent;
    char send_data[1024];
    MyserverAList *node = anchor; 
    char cost[10];
    
    
    
    if((tcp_sfd = socket(AF_INET,SOCK_STREAM,0))==-1) {
                error("Socket");
        }
    server_tcp.sin_family = AF_INET;
    server_tcp.sin_port = htons(SERVER_TCP_PORT);
    server_tcp.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_tcp.sin_zero),8);
    usleep(7000000);
    if((connect(tcp_sfd,(struct sockaddr *)&server_tcp,sizeof(server_tcp)))==-1) {
        error("server: Connect");
    }
        
    /*printf("Enter message:\t");
    fgets(send_data,1024,stdin); */

    while(node->next != NULL) {
        
        node = node->next;
        memset(send_data,0,sizeof(send_data));
        strcat(send_data,((serverA_t *)node->obj)->host_name);
        sprintf(cost,"%d",((serverA_t *)node->obj)->cost_link);
        strcat(send_data,"\t");
        strcat(send_data,cost);
        usleep(7000000);
        /* send the server's neighbour information to client */
        bytes_sent = send(tcp_sfd,send_data,strlen(send_data),0);
        if(bytes_sent < 0) {
            error("server: send");
        }
    }
    send(tcp_sfd,"exit",10,0);
    
    /*printf("Receiving data from server\n");*/
    
    /*bytes_received = recv(tcp_sfd,recv_data,1024,0);
    recv_data[bytes_received] = '\0';
    printf("\nReceived Data: %s",recv_data); */
    
    close(tcp_sfd); 
}

void myserverA_list_init(MyserverAList *head) {
        head->next = NULL;
        head->obj = NULL;
}

int main() {
        
        anchor = (MyserverAList *)malloc(sizeof(MyserverAList));
        myserverA_list_init(anchor);
        FILE *fp = NULL;
        
        fp = fopen("serverA.txt","r");
        if(fp == NULL) {
            error("File is empty!");
        } else {
            read_input_file(fp);
        }
        
    /*    display_list(); */
        
                
        
        
        fclose(fp);
        create_socket_udp(0);
        create_socket_tcp();
        /* For obtaining adjacency matrix from client side*/
        create_socket_udp(1);
        return 0;
}
