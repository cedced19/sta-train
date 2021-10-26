#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include  "../config.h"

#define TRAIN_ID 5

#define DISTANCE_PARCOURS 700 // Avance max d'un train pour le test

#define CHECK_ERROR_DIFFERENT(msg,val1,val2) if (val1 != val2) { perror(msg); exit(-1); }
#define CHECK_ERROR_EQUAL(msg,val1,val2) if (val1 == val2) { perror(msg); exit(-1); }
#define N_ARG(x) (argc == x) || ((argc == x + 1) && (argv[argc] == NULL))

//Definition d'un type train data
typedef struct TrainInfo
{
	float distance;  // en cm
	float vit_consigne;
	int vit_mesuree;
	int nb_impulsions;
	int id;
}TrainInfo;

int main(int argc, char *argv[])
{
	if (argc >= 3) {

		char *serverIp = argv[1];
        int serverPort = atoi(argv[2]);
        int sock;
		int trainid;
		if (argc == 4) {
			trainid = atoi(argv[3]);
		} else {
			trainid = TRAIN_ID;
		}

        struct sockaddr_in addr_rbc;
		printf("Connection en cours \n");
		// Création de la socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        CHECK_ERROR_EQUAL("Impossible de créer une socket", sock, -1);
		printf("Socket OK \n");
		bzero(&addr_rbc, sizeof(addr_rbc));

		
        // Définition du port et de l'ip 
        addr_rbc.sin_family = AF_INET;
        addr_rbc.sin_addr.s_addr = inet_addr(serverIp);
        addr_rbc.sin_port = htons(serverPort);

		CHECK_ERROR_DIFFERENT("Impossible de se connecter au serveur", connect(sock, (struct sockaddr*)&addr_rbc, sizeof(addr_rbc)), 0);
		
		printf("Connection OK \n");

		sendData(sock, 1, trainid, -1, -1);

		printf("Train auth OK \n");
		

		return 0;
	} else {
		printf("No ip and port specified.");
	}
}

