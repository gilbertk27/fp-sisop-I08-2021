#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8080

void mkdirectory(char destination_dir[100]) {
    char *argv[] = {"mkdir", "-p", destination_dir, NULL};
    execv("/bin/mkdir", argv);
}

void failedfork_exit(pid_t child_id) {
    if (child_id < 0)
        exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    pthread_t tid[1000];
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
	

    while(1){
        char inputted[10][200];
        char buffer[1024] = {0};
        valread = read(new_socket, buffer, 1024);

        char * split_inputted = strtok(buffer, " ");

        printf(" full %s\n",buffer);
        printf(" splitted\n");

        int count = 0;
        while(split_inputted != NULL){
            printf(" %s\n",split_inputted);

            strcpy(inputted[count], split_inputted);
            split_inputted = strtok(NULL, " ");
            count ++;
        }

        printf("done split\n");
        printf("%s -> %ld \n", inputted[0], strlen(inputted[0]));
        printf("%s\n", inputted[1]);

        int exec_childid = fork();
        failedfork_exit(exec_childid);

        char path_database[100] = "./databases";

        if(exec_childid == 0){
            if(strcmp(inputted[0], "create_db") == 0){
                strcat(path_database, "/");
                strcat(path_database,inputted[1]);
                mkdirectory(path_database);
            }
        }
    }
    return 0;
}
