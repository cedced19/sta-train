#include  "../config.h"

#define MAXTRAIN 6
#define GAIN 100
#define EMERGENCY_DIST 20
#define MAX_SPEED 30

typedef struct train {
	int id;
	int pos;
	int speed;
	int speedMeasured;
	int order;
	struct train *nextTrain;
} Train;

Train * addTrain (int id, int pos, int speed, Train *trains);
void showTrains(Train *trains);
static Train * newTrain(int id, int pos, int speed);
Train * selectTrain (int id_train, Train *trains);
void * storeData(int id, int pos, int speed, Train * trains);
float calcSpeed(Train *train, Train * trainsList);
void * orderTrain(Train * trains);
int calcMinDistance(int id_train, Train * trainsList);
int calcDistance(Train *train1, Train *train2);