#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

extern int errno;

int port;

int main(int argc, char *argv[]){
    int sd; //socket decriptor
    struct sockaddr_in server;

    //confirm argumets
    if(argc != 3){
        printf("Syntax: %s <server_address> <port>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    //creating socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("[Client]Error at socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    //connecting to server
    if(connect(sd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1){
        perror("[Client]Error at connect().\n");
        return errno;
    }

    char msg[10000];
    int bytes;
    while(1){
        bzero(msg, 10000);
        printf("[Client]Enter command: ");
        fflush(stdout);
        
        if((bytes = read(0, msg, 10000)) < 0){
            perror("[Client]Error at read() from server.\n");
            return errno;
        }

        if(write(sd, msg, 100) <= 0){
            perror("[Client]Error at write() to server.\n");
            return errno;
        }

        
        if(read(sd, msg, 10000) < 0){
            perror("[Client]Erroar at read() from server.\n");
            return errno;
        }
        
        if(strcmp(msg, "Disconnected Succesfully!\0") == 0){
            printf("[Client]Message got from the server: %s\n", msg);
            break;
        } else {
            printf("[Client]Message got frot the server: %s\n", msg);
        }
    }
    close(sd);
}

