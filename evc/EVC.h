
// Define type for train
typedef struct TrainInfo
{
	float position;  // en cm
	float vit_consigne;
	int vit_mesuree;
	int nb_impulsions;
	int positionDone;
}TrainInfo;

void sig_handler(int signo);
void stopTrain(void);
int WriteVitesseLimite(float vitesseLimite);
int WriteVitesseConsigne(unsigned int vitesse, unsigned char sense);
int  WriteTrameStatusRUNRP1(unsigned char status, unsigned char varDebug1, unsigned char varDebug2);
void TraitementDonnee (uCAN1_MSG *recCanMsg, TrainInfo *infos);
void readCANMsg(uCAN1_MSG *recCanMsg, TrainInfo *infos);
void* getCANMsg(void* arg);
