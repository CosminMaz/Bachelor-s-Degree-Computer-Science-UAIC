#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <utmp.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define fifoName "clientServerFifo"
#define SHARED_MEMORY "sharedMemory"
#define SHARED_MEMORY_SIZE 4096

int main() {
    printf("Server Side is ON\n");

    if ((mkfifo(fifoName, 0644) == -1) && errno != EEXIST) {
        perror("Error at mkfifo client");
        exit(-1);
    }

    int* logged = mmap(NULL, sizeof(int), 
                       PROT_READ  | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(logged == MAP_FAILED) {
        perror("Error at mmap server");
        exit(-1);
    }
    *logged = 0;

    int* running = mmap(NULL, sizeof(int),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(running == MAP_FAILED){
        perror("Error at mmap server");
        exit(-1);
    }
    *running = 1;

    int shm = shm_open(SHARED_MEMORY, O_CREAT | O_RDWR, 0666);
    if(shm == -1){
        perror("Error at shm_open server");
        exit(-1);
    }
    if(ftruncate(shm, SHARED_MEMORY_SIZE) == -1) {
        perror("Error at ftruncate server");
        exit(-1);
    }

    char* acc = mmap(NULL, SHARED_MEMORY_SIZE, 
                     PROT_WRITE,
                     MAP_SHARED,
                     shm, 0);
    if(acc ==  MAP_FAILED){
        perror("Error at mmap server");
        exit(-1);
    }
    strcpy(acc, "guest");

    int sockp[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0) {
        perror("Error at socketpair server");
        exit(-1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error at fork in server");
        exit(-1);
    }

    // Main Child Process
    if (pid == 0) {
        close(sockp[1]);
        do{
            int pip[2];
            if (pipe(pip)) {
                perror("Error at pipe client");
                exit(-1);
            }
            pid_t pid2 = fork();
            if (pid2 == -1) {
                perror("Error at fork child server");
                exit(-1);
            }

            //Second Child Process
            if (pid2 == 0) {
                char childBuffer[1024];
                int childLength;
                char message[1024];
                close(pip[0]);
                childLength = read(sockp[0], childBuffer, sizeof(childBuffer));
                if (childLength < 0) {
                    perror("Error reading socketpair server");
                    exit(-1);
                } else {
                    childBuffer[childLength] = '\0';
                }

                if(strncmp(childBuffer, "login : ", 8) == 0) {
                    char temp[100];

                    if((*logged) == 1){
                        strcpy(temp, "You are already logged. Please logout before accesing other account");
                        sprintf(message, "[%ld] %s\n", strlen(temp) - 1, temp);
                        write(pip[1], message, strlen(message));
                        exit(0);
                    }

                    FILE* fd = fopen("users.pseudoDataBase", "r");
                    if(fd == NULL){
                        perror("Error at opening users client");
                        exit(-1);
                    }
                    while(fgets(temp, sizeof(temp), fd) != NULL) {
                        if(strcmp(childBuffer + 8, temp) == 0){
                            strncpy(acc, temp, strcspn(temp, "\n"));
                            strcpy(temp, "You succesfuly logged as ");
                            strcat(temp, childBuffer + 8);
                            sprintf(message, "[%ld] %s", strlen(temp) - 1, temp);
                            write(pip[1], message, strlen(message));
                            close(pip[1]);
                            //temp[strcspn(temp, "\n")] = '\0'; 
                            *logged = 1;
                            exit(0);
                        }
                    }
                    strcpy(temp, "User not found");
                    sprintf(message, "[%ld] %s\n", strlen(temp), temp);
                    write(pip[1], message, strlen(message));
                    close(pip[1]);
                    exit(0);
                }            

                if(strcmp(childBuffer,"logout\n\0") == 0) {
                    char temp[100];
                    strcpy(acc, "guest");
                    strcpy(temp, "You logout from the account");
                    sprintf(message, "[%ld] %s\n", strlen(temp) - 1, temp);
                    *logged = 0;
                    write(pip[1], message, strlen(message));
                    close(pip[1]);
                    exit(0);
                }

                if(strcmp(childBuffer, "get-logged-users\n\0") == 0){
                    char temp[500];
                    if((*logged) == 0){
                        strcpy(temp, "You must be logged to use this command");
                        sprintf(message, "[%ld] %s\n", strlen(temp) - 1, temp);
                        write(pip[1], message, strlen(message));
                        close(pip[1]);
                        exit(0);
                    }
                    setutent();
                    struct utmp* users; 
                    
                    while((users = getutent()) != NULL) {
                        if(users->ut_type == USER_PROCESS){
                            time_t loginTime = users->ut_tv.tv_sec;
                            sprintf(temp, "Username: %s\n      Hostname for remote login: %s\n      Time entry was made: %s", users->ut_user, users->ut_host, ctime(&loginTime));
                        }
                    }
                    endutent();

                    sprintf(message, "[%ld] %s", strlen(temp) - 1, temp);
                    write(pip[1], message, strlen(message));
                    close(pip[1]);
                    exit(0);
                }

                if(strncmp(childBuffer, "get-proc-info : ", 16) == 0){
                    char temp[100];
                    if((*logged) == 0){
                        strcpy(temp, "You must be logged to use this command");
                        sprintf(message, "[%ld] %s\n", strlen(temp) - 1, temp);
                        write(pip[1], message, strlen(message));
                        close(pip[1]);
                        exit(0);
                    }
                    char filePath[100];
                    printf("%s\n", childBuffer + 16);
                    sprintf(filePath, "/proc/");
                    strncat(filePath, childBuffer + 16, strcspn(childBuffer + 16, "\n"));
                    strcat(filePath, "/status");
                    printf("%s\n", filePath);
                    FILE* fd = fopen(filePath, "r");
                    if(fd == NULL && errno != ENOENT) {
                        perror("Error error at fopen server");
                        exit(-1);
                    }
                    if(errno == ENOENT){
                        strcpy(temp, "PID not found");
                        sprintf(message, "[%ld] %s\n", strlen(temp) - 1, temp);
                        write(pip[1], message, strlen(message));
                        close(pip[1]);
                        exit(0);
                    } 
                    char pid[50];
                    int hasVmSize = 0;
                    while(fgets(pid, sizeof(pid), fd) != NULL){
                        if(strncmp(pid, "Name: ", 5) == 0){
                            //strcat(temp, pid);
                            //strcat(temp, "     ");
                            sprintf(temp, "%s     ", pid);
                        }

                        if(strncmp(pid, "State: ", 6) == 0){
                            strcat(temp, pid);
                            strcat(temp, "     ");
                        }

                        if(strncmp(pid, "PPid: ", 5) == 0){
                            strcat(temp, pid);
                            strcat(temp, "     ");
                        }
                        
                        if(strncmp(pid, "Uid: ", 4) == 0){
                            strcat(temp, pid);
                            strcat(temp, "     ");
                        }

                        if(strncmp(pid, "VmSize: ", 7) == 0){
                            strcat(temp, pid);
                            hasVmSize = 1;
                        }
                    }

                    if(!hasVmSize){
                        strcat(temp, "VmSize:    N/A\n");
                    }

                    sprintf(message, "[%ld] %s", strlen(temp), temp);
                    write(pip[1], message, strlen(message));
                    close(pip[1]);
                    fclose(fd);
                    exit(0);
                }

                if(strncmp(childBuffer,"exit\0", 4) == 0){
                    
                    char temp[100];
                    strcpy(temp, "You exit the aplication\nServer Side turned OFF\nClient Side turned OFF\n");
                    sprintf(message, "[%ld] %s", strlen(temp) - 1, temp);
                    *running = 0;
                    write(pip[1], message, strlen(message));
                    close(pip[1]);
                    exit(0);
                }

                char temp[100];
                strcpy(temp, "Command does not exist");
                sprintf(message, "[%ld] %s\n", strlen(temp) - 1, temp);
                write(pip[1], message, strlen(message));
                close(pip[1]);
                exit(0); 
            }

            //Second Parent Process
            if(pid2 > 0){
                wait(NULL);
                char parentBuffer[1024];
                int parentLength;
                close(pip[1]);
                parentLength = read(pip[0], parentBuffer, sizeof(parentBuffer));
                if(parentLength < 0) {
                    perror("Error at reading from pipe server");
                    exit(-1);
                } else 
                    parentBuffer[parentLength] = '\0';

                
                close(pip[0]);
                write(sockp[0], parentBuffer, parentLength);
            }
        }while((*running));

        close(sockp[0]); 
        exit(0);
    }

    // Main Parent Process
    if (pid > 0) {
        char buffer[1024];

        int fd = open(fifoName, O_RDWR);
        if (fd == -1) {
            perror("Error opening FIFO on server");
            exit(-1);
        }

        close(sockp[0]);  

        do{
            sleep(1);
            int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                //printf("Parent read from FIFO: %s\n", buffer);
                //fflush(stdout);

                if (write(sockp[1], buffer, bytesRead) < 0) {
                    perror("Error writing to socket server");
                    exit(-1);
                }
                bytesRead = read(sockp[1], buffer, sizeof(buffer));
                if (bytesRead > 0)
                    buffer[bytesRead] = '\0';
                
                
                write(fd, buffer, bytesRead);
            }
        }while((*running));

        close(fd);  
        close(sockp[1]);
    }

    if(munmap(acc, SHARED_MEMORY_SIZE) == -1) {
        perror("Error at munmap server");
        exit(-1);
    }

    if(munmap(logged, sizeof(logged)) == -1) {
        perror("Error at munmap server");
        exit(-1);
    }

    if(munmap(running, sizeof(running)) == -1){
        perror("Error at munmap server");
        exit(-1);
    }

    if(close(shm) == -1){
        perror("Error at closing shared memory server");
        exit(-1);
    }

    if(shm_unlink(SHARED_MEMORY) == -1) {
        perror("Erroar at unlinking shared map server");
        exit(-1);
    }   

    return 0;
}
