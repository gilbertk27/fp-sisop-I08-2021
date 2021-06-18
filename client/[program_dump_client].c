#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int createSocket(){
    int socket_netwrk;
    if ((socket_netwrk = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    return socket_netwrk;
}

void sqlCmdCreate(int socket_netwrk){
    char package[300]={0};
    char command[20]={0};
    scanf(" %s", command);

    if(strcmp(command, "DATABASE") == 0){
        char dbsName[255]={0};
        scanf(" %s", dbsName);

        strcpy(package, "create_db ");
        strcat(package, dbsName);

        /*create_db [nama_db]*/


        send(socket_netwrk , package , strlen(package) , 0);
    }
}

int main(int argc, char const *argv[]) {

    /*SOCKET CONFIGURATION*/
    struct sockaddr_in address;

    int socket_netwrk = createSocket();
    int valread;

    struct sockaddr_in serv_addr;

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(socket_netwrk, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    /*SOCKET CONFIGURATION END*/

    char command[20]={0};
    while(1){
        command[0] = '\0';
        scanf(" %s", command);

        if(strcmp(command, "CREATE") == 0){
            sqlCmdCreate(socket_netwrk);
            continue;
        }else{
            printf("-----------------------------------------------------\n");
            printf("command %s is not provided\n", command);
            printf("please try this following format: CREATE DATABASE <database_name> \n\n");
            printf("to make a database\n");

        }


    }

    return 0;
}
