#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <fcntl.h>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

#include "Command.h"
#include "XMLClass.h"

#define PORT 3000
#define MAX_THREADS 10
#define MAX_CLIENTS 10000
#define BUFFER_SIZE 1024

struct cl{
    int client_fd;
    char command_buff[BUFFER_SIZE];
};

// Queues
std::queue<cl> clients_queue;
std::queue<Command> tasks_queue;

//Client queue mutex
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_condition = PTHREAD_COND_INITIALIZER;

//Task queue mutex
pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t task_condition = PTHREAD_COND_INITIALIZER;

// Thread pool
std::vector<pthread_t> thread_pool;


// Function prototypes
void* client_handler(void* arg);
void* command_handler(void* arg);

void send_message(int client, char msg[10000]);

int set_nonblocking(int fd);

int main(){
    
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Initialize server socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("[Server]Error at socket()\n");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("[Server]Error at setsockopt()\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("[Server]Error at bind()\n");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, MAX_CLIENTS) < 0) {
        perror("[Server]Error at listen()");
        exit(EXIT_FAILURE);
    }

    // Set server socket to non-blocking
    set_nonblocking(server_fd);

    fd_set clients_set, read_set;
    FD_ZERO(&clients_set);
    FD_SET(server_fd, &clients_set);
    int max_sd = server_fd;

    //Threads pool
    for(int i = 0; i < MAX_THREADS; ++i) {
        pthread_t thread;
        pthread_create(&thread, nullptr, client_handler, nullptr);
        thread_pool.push_back(thread);
    }

    //Command handler thread
    pthread_t command_thread;
    pthread_create(&command_thread, nullptr, command_handler, nullptr);

    printf("[Server]Server is listening on port %d\n", PORT);

    while(true) {
        read_set = clients_set;

        int activity = select(max_sd + 1, &read_set, nullptr, nullptr, nullptr);

        if (activity < 0 && errno != EINTR) {
            perror("[Server]Error at select()\n");
            exit(EXIT_FAILURE);
        }

        // Check for new connections
        if (FD_ISSET(server_fd, &read_set)) {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("[Server]Error at accept()\n");
                continue;
            }
            printf("[Server]New connection: client fd is %d, ip is %s, port %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            
            set_nonblocking(new_socket);
            FD_SET(new_socket, &clients_set);
            max_sd = std::max(max_sd, new_socket);
        }

        // Check for IO on existing sockets
        for (int i = 0; i <= max_sd; ++i) {
            if (FD_ISSET(i, &read_set) && i != server_fd) {
                char buffer[1024] = {0};
                int bytes = read(i, buffer, BUFFER_SIZE);

                if (bytes == 0) {
                    // Client disconnected
                    printf("[Server]Client disconnected, socket fd: %d\n", i);
                    close(i);
                    FD_CLR(i, &clients_set);
                } else {
                    buffer[bytes] = '\0';
                    printf("[Server]Received from client %d: %s", i, buffer);
                    cl new_client;
                    new_client.client_fd = i;
                    strcpy(new_client.command_buff, buffer);
                    strcat(new_client.command_buff, "\0");
                    pthread_mutex_lock(&client_mutex);
                    clients_queue.push(new_client);
                    pthread_cond_signal(&client_condition);
                    pthread_mutex_unlock(&client_mutex);
                }
            }
        }
    }

    close(server_fd);
    for(auto& thread : thread_pool) {
        pthread_cancel(thread);
    }
    pthread_cancel(command_thread);
    pthread_mutex_destroy(&client_mutex);
    pthread_cond_destroy(&client_condition);
    pthread_mutex_destroy(&task_mutex);
    pthread_cond_destroy(&task_condition);

    return 0;
}

void handle_client(cl client_fd) {
    Command com(client_fd.command_buff, client_fd.client_fd);
    pthread_mutex_lock(&task_mutex);
    tasks_queue.push(com);
    pthread_mutex_unlock(&task_mutex);
    pthread_cond_signal(&task_condition);
}

void* client_handler(void* arg) {
    while(true) {
        pthread_mutex_lock(&client_mutex);

        while(clients_queue.empty()) {
            pthread_cond_wait(&client_condition, &client_mutex);
        }
        cl client = clients_queue.front();
        clients_queue.pop();
        pthread_mutex_unlock(&client_mutex);

        handle_client(client);
    }
    return nullptr;
}

void* command_handler(void* arg) {
    char file[] = "trains.xml";
    XMLClass xml_file(file);
    while(true) {
        pthread_mutex_lock(&task_mutex);
        while(tasks_queue.empty()) {
            pthread_cond_wait(&task_condition, &task_mutex);
        }

        Command task = tasks_queue.front();
        tasks_queue.pop();
        pthread_mutex_unlock(&task_mutex);

        // Process the command
        if(task.GetFlag() == 1){
            char response[1000]; 
            strcpy(response, xml_file.GetTodayTrains());
            send_message(task.GetClient(), response);
        } else if(task.GetFlag() == 2){
            char response[1000]; 
            strcpy(response, xml_file.GetThisHourDepartures(task.GetCurrentHour()));
            send_message(task.GetClient(), response);
        } else if(task.GetFlag() == 3){
            char response[1000]; 
            strcpy(response, xml_file.GetThisHourArrivals(task.GetCurrentHour()));
            send_message(task.GetClient(), response);
        } else if(task.GetFlag() == 4){
            char response[1000]; 
            strcpy(response, xml_file.showInfo(task.GetTrainId()));
            send_message(task.GetClient(), response);
        } else if(task.GetFlag() == 5){
            char response[1000]; 
            strcpy(response, xml_file.addDelay(task.GetTrainId(), task.GetDelay(), task.GetStation()));
            send_message(task.GetClient(), response);
        } else if(task.GetFlag() == 6) {
            char response[1000]; 
            strcpy(response, xml_file.addEarly(task.GetTrainId(), task.GetDelay(), task.GetStation()));
            send_message(task.GetClient(), response);
        } else if(task.GetFlag() == 7){
            char response[1000];
            strcpy(response, "Disconnected Succesfully!\0");
            send_message(task.GetClient(), response);
        } else {
            char response[1000];
            strcpy(response, "Invalid command.\n");
            send_message(task.GetClient(), response);
        }
    }
    return nullptr;
}


int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void send_message(int client, char msg[10000]){
    int len = write(client, msg, strlen(msg));
    if (len < 0) {
        perror("[Server] Error at write().\n");
        close(client);
        } else {
            printf("[Command Handler] Response sent to client %d.\n", client);
        }
}