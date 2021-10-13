#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Libs_Unirail/CAN/canLinux.h"
#include "Libs_Unirail/CAN/loco_Parametres.h"



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
}TrainInfo;

unsigned char status, varDebug1, varDebug2;

//////////////////////////////////////////
/// Ecriture de la Trame vitesse limite
///////////////////////////////////////////
int WriteVitesseLimite(float vitesseLimite)
{

	uCAN1_MSG consigneUrgence;
	
	if(vitesseLimite > MAX_CONSIGNE_VITESSE_AUTORISEE) //vitesse sup�rieur � 50 cm/s
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
		/** Envoi la vitesse instantann�e (consigne vitesse) ,	le nombre d�impulsions, la vitesse mesur�e, l�erreur du PID **/

		if(MESCAN_GetData8(recCanMsg, cdmc_ordonnancementId)==MC_ID_RP1_STATUS_RUN)
			WriteTrameStatusRUNRP1(status, varDebug1, varDebug2);
			
        infos-> vit_mesuree= (int)MESCAN_GetData8(recCanMsg, cdmc_vitesseMesuree);/** le nbre d'implusion envoy� ici
		// est le nombre d'impulsion entre 2 mesures **/
		infos-> nb_impulsions+= infos-> vit_mesuree;
        infos-> distance= PAS_ROUE_CODEUSE * (infos->nb_impulsions);
		infos-> vit_consigne= (float)MESCAN_GetData8(recCanMsg, cdmc_vitesseConsigneInterne);
		printf("Actualisation: Postition courante : %lf cm, Vit: %d cm/s\n", infos-> distance, infos-> vit_mesuree);
	}
	else 
		printf("La trame lue a pour ID %X \n",recCanMsg->frame.id);
}


void sendMessage(int sock, char message[]) {
    send(sock, message, strlen(message), 0);
}


////////////////////////////////////////
/// MAIN
////////////////////////////////////////
int main(int argc, char *argv[])
{
	if (N_ARG(2)) {
		char *serverIp = argv[1];
        int serverPort = atoi(argv[2]);
        int sock;

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

		sendMessage(sock, "1");

		printf("Train authentifié \n");
		
		//Definition d'une variable de type message can
		uCAN1_MSG recCanMsg;
		
		//Definition d'une variable pour stocker la distance parcourue et la vitesse du train
		struct TrainInfo train1;
		// Definition du nom de l'interface can du raspberry pi. A controler au niveau systeme.
		char *NomPort = "can0";
		// Definition d'une variable pour memoriser le descripteur de port CAN ouvert
		int canPort;
		//Definition d'un filtre CAN pour preciser les identifiant a lire
		struct can_filter rfilter[1]; //Le filtre sera limite ici a une variable

		//Initialisation du filtre
		rfilter[0].can_id   = MC_ID_SCHEDULEUR_MESURES; //On indique que l'on veut lire ces trames CAN
		rfilter[0].can_mask = CAN_SFF_MASK;
		
		train1.distance =0;
		train1.vit_consigne =0;
		train1.vit_mesuree =0;
		train1.nb_impulsions =0;
		
	
		printf("PROGRAMME DE TEST DE L'AVANCE D'UN TRAIN !!! \n");

		
		// Creation du descripteur de port a utilise pour communiquer sur le bus CAN
		canPort = canLinux_init_prio(NomPort);
		// Mise en place d'un filtre
		canLinux_initFilter(rfilter, sizeof(rfilter));
	
		// Deverouillage de la limite de vitesse autorisee
		WriteVitesseLimite(MAX_CONSIGNE_VITESSE_AUTORISEE);+
		usleep(150000); //150ms
		
		while(train1.distance < DISTANCE_PARCOURS)
		{
			if(canLinux_receive(&recCanMsg, 1)) //Lecture d'une trame CAN
			{
				printf("Lecture trame CAN.\n");
				//Traitement de la trame pour evaluer la distance de deplacement du train
				TraitementDonnee(&recCanMsg, &train1); 
				//Ecriture d'une nouvelle consigne de vitesse
				WriteVitesseConsigne(((DISTANCE_PARCOURS-train1.distance)/2)+1, 1);
				
			}
		usleep(15000); //Sampling period ~= 17ms
		}
		WriteVitesseConsigne(0, 1); //Arret du train en envoyant une consigne de distance nulle

		//fermeture du port CAN
		canLinux_close();
		printf("FIN DU PROGRAMME D'AVANCE DU TRAIN !!!\n");

		return 0;
	}
}

