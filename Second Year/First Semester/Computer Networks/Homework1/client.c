#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define fifoName "clientServerFifo"
#define SHARED_MEMORY "sharedMemory"
#define SHARED_MEMORY_SIZE 4096

int main() {
    printf("Client Side is ON\n");
    
    int* running = mmap(NULL, sizeof(running),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(running == MAP_FAILED){
        perror("Erroar at mmap client");
        exit(-1);
    }
    *running = 1;

    int shm = shm_open(SHARED_MEMORY, O_RDWR, 0666);
    if(shm == -1){
        perror("Error at shm_open client");
        exit(-1);
    }

    char* acc = mmap(NULL, SHARED_MEMORY_SIZE, 
                     PROT_READ, 
                     MAP_SHARED,
                     shm, 0);
    if(acc == MAP_FAILED){
        perror("Error at mmap client");
        exit(-1);
    }

    // Main Fork
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error at main fork client");
        exit(-1);
    }

    // Main Child Process
    if (pid == 0) {
        do{
            int pip[2];
            if (pipe(pip)) {
                perror("Error at pipe client");
                exit(-1);
            }

            // Second Fork
            pid_t pid2 = fork();
            if (pid2 == -1) {
                perror("Error at second fork client");
                exit(-1);
            }

            // Second Child Process
            if (pid2 == 0) {
                // Third Fork
                pid_t pid3 = fork();
                if (pid3 == -1) {
                    perror("Error at third fork client");
                    exit(-1);
                }

                // Third Child Process
                if (pid3 == 0) {
                    int childLength;
                    char childBuffer[1024];
                    printf("<%s>$", acc);
                    fflush(stdout);

                    childLength = read(0, childBuffer, sizeof(childBuffer));
                    childBuffer[childLength] = '\0';
                    //printf("You entered: %s", childBuffer);
                    //fflush(stdout);

                    close(pip[0]);
                    write(pip[1], childBuffer, childLength);
                    close(pip[1]); 
                    exit(0);
                }

                // Third Parent Process
                if (pid3 > 0) {
                    wait(NULL); 
                    int parentLength;
                    char parentBuffer[1024];

                    close(pip[1]);
                    parentLength = read(pip[0], parentBuffer, sizeof(parentBuffer));

                    parentBuffer[parentLength] = '\0';
                    close(pip[0]); 

                    int fd = open(fifoName, O_WRONLY);
                    if (fd == -1) {
                        perror("Error at opening FIFO client");
                        exit(-1);
                    }

                    write(fd, parentBuffer, parentLength);
                    close(fd); 
                    exit(0);  
                }
            }

            // Second Parent Process
            if (pid2 > 0) {
                wait(NULL); 
                char parentBuffer[1024];
                int parentLength;
    
                int fd = open(fifoName, O_RDONLY);
                if (fd == -1) {
                    perror("Error at opening FIFO client");
                    exit(-1);
                }

                parentLength = read(fd, parentBuffer, sizeof(parentBuffer));
                close(fd);
                if(parentLength < 0){
                    perror("Error at reading from fifo");
                    exit(-1);
                } else {
                    parentBuffer[parentLength] = '\0';
                }
                printf("%s", parentBuffer);
                fflush(stdout);

                if(memcmp(parentBuffer + 5, "You exit\0", 8) == 0){
                    *running = 0;
                }
            }
        }while((*running));
    }

    // Main Parent Process
    if (pid > 0) {
        while (wait(NULL) != -1 || errno != ECHILD);
    }

    if(munmap(running, sizeof(running)) == -1) {
        perror("Error at munmap client");
        exit(-1);
    }

    if(munmap(acc, SHARED_MEMORY_SIZE) == -1) {
        perror("Error at munmap client");
        exit(-1);
    }

    if(close(shm) == -1) {
        perror("Error at closing shared memory client");
        exit(-1);
    }

    return 0;
}
