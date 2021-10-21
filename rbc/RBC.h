#include  "../config.h"

#define MAXTRAIN 6

typedef struct train {
	int id;
	int pos;
	int speed;
	
	struct train *nextTrain;
} Train;

Train * addTrain (int id, int pos, int speed, Train *trains);
void showTrains(Train *trains);
static Train * newTrain(int id, int pos, int speed);
Train * selectTrain (int id_train, Train *trains);
void * storeData(int id, int pos, int speed, Train * trains);