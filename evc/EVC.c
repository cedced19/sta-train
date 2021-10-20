#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "Libs_Unirail/CAN/canLinux.h"
#include "Libs_Unirail/CAN/loco_Parametres.h"
#include "EVC.h"

#include  "../config.h"

#define TRAIN_ID 1

#define position_PARCOURS 700 // Avance max d'un train pour le test

#define CHECK_ERROR_DIFFERENT(msg,val1,val2) if (val1 != val2) { perror(msg); exit(-1); }
#define CHECK_ERROR_EQUAL(msg,val1,val2) if (val1 == val2) { perror(msg); exit(-1); }
#define N_ARG(x) (argc == x) || ((argc == x + 1) && (argv[argc] == NULL))

struct TrainInfo train1;

unsigned char status, varDebug1, varDebug2;

//////////////////////////////////////////
/// Signal handler
///////////////////////////////////////////
void sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("received SIGINT\n");
		train1.emergencyStop = 1;
        stopTrain();
        exit(0);
    }
}

//////////////////////////////////////////
/// Stop train
///////////////////////////////////////////
void stopTrain(void) {
    printf("Stop train");
    WriteVitesseConsigne(0, 1);
}

//////////////////////////////////////////
/// Ecriture de la Trame vitesse limite
///////////////////////////////////////////
int WriteVitesseLimite(float vitesseLimite)
{

	uCAN1_MSG consigneUrgence;
	
	if(vitesseLimite > MAX_CONSIGNE_VITESSE_AUTORISEE) //vitesse supérieur à 50 cm/s
	vitesseLimite = MAX_CONSIGNE_VITESSE_AUTORISEE;                   
	
	consigneUrgence.frame.id  = MC_ID_CONSIGNE_VITESSE_LIMITE;
	consigneUrgence.frame.dlc = MC_DLC_CONSIGNE_VITESSE_LIMITE;
	MESCAN_SetData8(&consigneUrgence, cdmc_consigneVitesseLimite, vitesseLimite);
	
	return 	canLinux_transmit(CANLINUX_PRIORITY_HIGH, &consigneUrgence);
}

//////////////////////////////////////////
/// Ecriture de la Trame Consigne
///////////////////////////////////////////
int WriteVitesseConsigne(unsigned int vitesse, unsigned char sense)
{
	
	uCAN1_MSG consigneVitesse;

	if(vitesse>MAX_CONSIGNE_VITESSE_AUTORISEE) //vitesse sup�rieur � 50 cm/s
		vitesse = MAX_CONSIGNE_VITESSE_AUTORISEE;
	
	consigneVitesse.frame.id  = MC_ID_CONSIGNE_VITESSE;
	consigneVitesse.frame.dlc = MC_DLC_CONSIGNE_VITESSE;
	MESCAN_SetData8(&consigneVitesse, cdmc_consigneVitesse, vitesse);
	MESCAN_SetBit(&consigneVitesse, bdmc_sensDeplacementLoco, sense);
	
	return 	canLinux_transmit(CANLINUX_PRIORITY_HIGH, &consigneVitesse);
}


//////////////////////////////////////////
/// Envoi de la trame status de RPI1
///////////////////////////////////////////
int  WriteTrameStatusRUNRP1(unsigned char status, unsigned char varDebug1, unsigned char varDebug2)
{
	uCAN1_MSG trameStatusRP1;
	trameStatusRP1.frame.id  = MC_ID_RP1_STATUS_RUN;
	trameStatusRP1.frame.dlc = MC_DLC_RP1_STATUS_RUN;
	MESCAN_SetData8(&trameStatusRP1, cdmc_RP1_erreurs, 0);
	MESCAN_SetData8(&trameStatusRP1, cdmc_RP1_warnings, 0);
	MESCAN_SetBit(&trameStatusRP1, bdmc_RP1_etatConnexionWIFI, 1);
	MESCAN_SetData8(&trameStatusRP1, cdmc_RP1_configONBOARD, status);
	MESCAN_SetData8(&trameStatusRP1, cdmc_RP1_var1Debug, varDebug1);
	MESCAN_SetData8(&trameStatusRP1, cdmc_RP1_var2Debug, varDebug2);
	
	return 	canLinux_transmit(CANLINUX_PRIORITY_MEDIUM, &trameStatusRP1);
}

//////////////////////////////////////////
/// Traitement d'une trame CAN
///////////////////////////////////////////
void TraitementDonnee (uCAN1_MSG *recCanMsg, TrainInfo *infos)
{
	
		
    if (recCanMsg->frame.id==MC_ID_SCHEDULEUR_MESURES)
    {
		// Envoi la vitesse instantannée (consigne vitesse) ,	le nombre d''impulsions, la vitesse mesurée, l'erreur du PID

		if(MESCAN_GetData8(recCanMsg, cdmc_ordonnancementId)==MC_ID_RP1_STATUS_RUN)
			WriteTrameStatusRUNRP1(status, varDebug1, varDebug2);
			
        infos-> vit_mesuree= (int)MESCAN_GetData8(recCanMsg, cdmc_vitesseMesuree);/** le nbre d'implusion envoyé ici
		// est le nombre d'impulsion entre 2 mesures **/
		infos-> nb_impulsions+= infos-> vit_mesuree;
        infos-> position= PAS_ROUE_CODEUSE * (infos->nb_impulsions);
		infos-> vit_consigne= (float)MESCAN_GetData8(recCanMsg, cdmc_vitesseConsigneInterne);
		printf("Actualisation: Postition courante : %lf cm, Vit: %d cm/s\n", infos-> position, infos-> vit_mesuree);
	}
	else 
		printf("La trame lue a pour ID %X \n",recCanMsg->frame.id);
}


//////////////////////////////////////////
/// Lecture des trames CAN
///////////////////////////////////////////
void readCANMsg(uCAN1_MSG *recCanMsg, TrainInfo *infos)
{	
	int baliseCode;
	int positionFromBalise = -1;
	if (recCanMsg->frame.id==MC_ID_SCHEDULEUR_MESURES) {
		// Envoi la vitesse instantannée (consigne vitesse) ,	le nombre d''impulsions, la vitesse mesurée, l'erreur du PID

		if(MESCAN_GetData8(recCanMsg, cdmc_ordonnancementId)==MC_ID_RP1_STATUS_RUN) {
			//WriteTrameStatusRUNRP1(status, varDebug1, varDebug2);
			// je ne sais pas pourquoi mais cela éteint le raspberry
		}
		infos-> vit_mesuree= (int)MESCAN_GetData8(recCanMsg, cdmc_vitesseMesuree);
		// le nbre d'implusion envoyé ici
		// est le nombre d'impulsion entre 2 mesures 
		infos-> nb_impulsions+= infos-> vit_mesuree;
        infos-> position+= PAS_ROUE_CODEUSE * (infos->nb_impulsions);
		infos-> vit_consigne= (float)MESCAN_GetData8(recCanMsg, cdmc_vitesseConsigneInterne);
		//printf("Actualisation: Postition courante : %lf cm, Vit: %d cm/s\n", infos-> position, infos-> vit_mesuree);
		//printf("Position: %lf\n", infos-> position);
	} else if (recCanMsg->frame.id==MC_ID_EBTL2_RECEIVED) {
		//printf("Balise numéro %X\n", recCanMsg->frame.data5);
		infos->positionDone = 1;
		baliseCode = recCanMsg->frame.data5;
		switch(baliseCode) {
			case 1:
				positionFromBalise = 0;
				break;
			case 2:
				positionFromBalise = 1671;
				break;
			case 3:
				positionFromBalise = 1898 + 1671;
				break;
			case 4:
				positionFromBalise = 1898 + 1671 + 2264;
				break;
			case 5:
				positionFromBalise = 1898 + 1671 + 2264 + 1617;
				break;
			case 6:
				positionFromBalise = 1898 + 1671 + 2264 + 1617 + 1612;
				break;
			case 7:
				positionFromBalise = 1898 + 1671 + 2264 + 1617 + 1612 + 2134;
				break;
			case 8:
				positionFromBalise = 1898 + 1671 + 2264 + 1617 + 1612 + 2134 + 1835;
				break;
			case 9:
				positionFromBalise = 1898 + 1671 + 2264 + 1617 + 1612 + 2134 + 1835 + 1628;
				break;
		}
		//printf("Postion donné par balise %i\n", positionFromBalise);
		if (positionFromBalise != -1) {
			infos->position = positionFromBalise;
			infos-> nb_impulsions = 0;
		}
	} else {
		printf("La trame lue a pour ID %X \n",recCanMsg->frame.id);
		
	}
	
}

//////////////////////////////////////////
/// Gestion des trames CAN
///////////////////////////////////////////
void* getCANMsg(void* arg) {

	uCAN1_MSG recCanMsg;
	int canPort;
	char *NomPort = "can0";
	struct can_filter rfilter[2]; 
	
	// Get mesures
	rfilter[0].can_id   = MC_ID_SCHEDULEUR_MESURES;
	rfilter[0].can_mask = CAN_SFF_MASK;
	
	// Get balise
	rfilter[1].can_id   = MC_ID_EBTL2_RECEIVED;
	rfilter[1].can_mask = CAN_SFF_MASK;

	//int consigne_rbc=20;

    /* Start CAN bus connexion */
    canPort = canLinux_init_prio(NomPort);
	canLinux_initFilter(rfilter, sizeof(rfilter));

    usleep(80000); //80ms
    
    while(1)
    {
		if(canLinux_receive(&recCanMsg, 1))
		{
			//printf("Reading trame CAN.\n");
			readCANMsg(&recCanMsg, &train1);
			//WriteVitesseConsigne(consigne_rbc, 1);
			
		}
    	usleep(8000); //Sampling period ~= 17ms
	}
	// if exit stop train
	WriteVitesseConsigne(0, 1);

    close(canPort);
	printf("EXIT CAN reading\n");
}


////////////////////////////////////////
/// MAIN
////////////////////////////////////////
int main(int argc, char *argv[])
{
	if (N_ARG(2)) {
		char data[MAX_MSG_SIZE];
		int readStatus;
		char *serverIp = argv[1];
        int serverPort = atoi(argv[2]);
        int sock;
		T_list list = NULL;

		// Message parsed
		int code;
		int id;
		int position;
		int speed;
		
		train1.position = 0;
		train1.vit_consigne = 0;
		train1.nb_impulsions = 0;
		train1.vit_mesuree = 0;
		train1.positionDone = 0;
		train1.emergencyStop = 0;

		struct sockaddr_in addr_rbc;
		pthread_t threadCAN;
		
		// Stop train when stopping program
		if (signal(SIGINT, sig_handler) == SIG_ERR)
        	printf("Cannot catch signal SIGINT\n");


		// thread to handle CAN messages
    	pthread_create(&threadCAN, NULL, getCANMsg, NULL);

        do {
			printf("Connection...\n");
			// Create socket
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == -1) {
				perror("Impossible de se connecter");
				continue;
			}
			printf("Socket OK \n");


			bzero(&addr_rbc, sizeof(addr_rbc));

			
			// Define port and ip address
			addr_rbc.sin_family = AF_INET;
			addr_rbc.sin_addr.s_addr = inet_addr(serverIp);
			addr_rbc.sin_port = htons(serverPort);

			if (connect(sock, (struct sockaddr*)&addr_rbc, sizeof(addr_rbc)) != 0) {
				perror("Unable to connect to the RBC");
				continue;
			}

			printf("Connection OK \n");

			if (train1.positionDone == 0) {
				sendData(sock, 1, TRAIN_ID, -1, 0);
			} else {
				sendData(sock, 1, TRAIN_ID, train1.position, 0);
			}
			

			// Need to wait for auth
			printf("Wait for auth\n");
			while (1) {
				data[0] = '\0';
				memset(data, 0, sizeof(data));
				readStatus = read(sock, data, MAX_MSG_SIZE);
				if (readStatus < 0) {
					perror("Error while reading");
				} else if (readStatus == 0) {
					perror("Connection closed while reading");
					break;
				} else {
					list = getOneMessage(list,data);
					while (list !=NULL) {

						parseMessage(list->data, &code, &id, &position, &speed);

						// Debug info
						/*
						printf("Code %d\n",code);
						printf("Id %d\n",id);
						printf("Position %d\n",position);
						printf("Speed %d\n",speed);
						*/

						if (id != TRAIN_ID) {
							list = removeFirstNode(list);
							continue;
						}

						switch (code) {
							case 2:
								printf("Train auth OK \n");
								if (train1.positionDone == 0) {
									printf("Looking for balise\n");
									while (train1.positionDone == 0 && train1.emergencyStop == 0) {
										WriteVitesseConsigne(8,1);
										sleep(1);
									}
									printf("One balise located\n");
									WriteVitesseConsigne(0,1);
									sendData(sock, 3, TRAIN_ID, (int)train1.position, train1.vit_mesuree);
								} else {
									printf("Nothing to do\n");
									WriteVitesseConsigne(0,1);
								}
								break;
							case 6:
								WriteVitesseConsigne(speed, 1);
								break;
							case 99:
								stopTrain();
								break;
						}

						//sendData(sock, 2, id, -1, -1);
						
						// sendToGUI(list->data)
						list = removeFirstNode(list);
					}
				}
			}
		} while(1);
		
		// Join threads
		pthread_join(threadCAN, NULL);

		return 0;
	} else {
		printf("No ip and port specified.");
	}
}

