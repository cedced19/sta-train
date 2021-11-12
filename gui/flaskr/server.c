// Default libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Others libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define LOCAL_PORT 4000
#define REMOTE_IP "127.0.0.1"
#define MAXCARS 100

#define CHECK_ERROR(msg, val1, val2) if(val1 == val2){perror(msg);exit(-1);}


int main(){

    struct sockaddr_in addrClient, addrServeur;
    char buffer[MAXCARS+1];
    int nbServer, sock1, sock2, len;
    // Creation de la socket
	sock1 = socket(AF_INET, SOCK_STREAM, 0);

	CHECK_ERROR("Erreur d'ouverture de socket\n", sock1, -1);

    // Définition de l'adresse du ecrivain
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(LOCAL_PORT);
	addrClient.sin_addr.s_addr = htonl(INADDR_ANY);

    // J'affecte l'adresse a la socket
	CHECK_ERROR("Erreur de Bind!!!\n", bind(sock1, (struct sockaddr *) &addrClient, sizeof(addrClient)), -1);	
	printf("Bind OK!!!\n");

    CHECK_ERROR("Erreur de listen!!!\n", listen(sock1, 1), -1);
    printf("Connexion socket OK!!!\n");

    // Accepté le client
    len = sizeof(addrClient);
    sock2 = accept(sock1, (struct sockaddr *) &addrClient, &len);
    if(sock2 < 0){
        printf("<%d>Le serveur pas accepté le client!!!\n", ntohs(addrClient.sin_addr.s_addr));
        exit(0);
    }else{
        printf("<%d>Le serveur accepté le client!!!\n", ntohs(addrClient.sin_addr.s_addr));
    }

    int position = 0;
    while(true){
        
        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "4:1:%d:10$$", position);
        nbServer = write(sock2, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
        
        usleep(100);

        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "4:2:%d:10$$", position + 500);
        nbServer = write(sock2, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));

        usleep(100);

        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "4:3:%d:10$$", position + 1000);
        nbServer = write(sock2, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));

        sleep(1);

        position += 500;

        if(position > 17600){
            position = 0;
        }


    }
    
    close(sock1);
}