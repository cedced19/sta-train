#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "RBC.h"
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define MAXLEN 500


#include "rtw/rtw_continuous.h"
#include "rtw/rtw_solver.h"
#include "rtwtypes.h"


#include "controller_1/controle_v7_1.h"      
#include "controller_2/controle_v7_2.h"   




pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

struct timeval lastCall;
struct timeval currentCall;

void onestep_1(Train *train);
void onestep_2(Train *train);

Train *trainsList=NULL;

float time_diff(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

//////////////////////////////////////////
/// Signal handler
/// SIGALARM
///////////////////////////////////////////
void periodSending(int signo) {
    Train* train = trainsList;
    //showTrains(train);
	while(train != NULL) {
        if (train->pos != -1 && train->id != 0) {
            if (train->initialized == 0) {
                printf("Initializing\n");
                if (train->id == 1) {
                    controle_v7_1_initialize();
                }
                if (train->id == 2) {
                    controle_v7_2_initialize();
                }
                train->initialized = 1;
            }
            if (train->id == 1) {
                onestep_1(train);
            }
            if (train->id == 2) {
                onestep_2(train);
            }
            
            sendData(train->sock, 6, train->id, train->pos, train->speed);
        }
        train = train->nextTrain;
    }
} 

void onestep_1(Train *train)
{
  static boolean_T OverrunFlag = false;
  gettimeofday(&currentCall, NULL);
  // time between last call
  printf("\nTime between last call of onestep: %0.8f s\n", time_diff(&lastCall, &currentCall));

  printf("\nonestep_1 begin\n");
  printf("Current train id: %d\n", train->id);

  

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(controle_v7_1_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */
  //if(train->order > )
  //train->pos-selectTrainByOrder()
  controle_v7_1_U.Distance = 200;
  controle_v7_1_U.Vitesse_Consigne = 20;
  controle_v7_1_U.Vitesse_Reelle = train->speedMeasured;
  controle_v7_1_U.Light = 0;

  // Log all inputs
  printf("Distance %f\n", controle_v7_1_U.Distance);
  printf("Vitesse Consigne (Input) %f\n", controle_v7_1_U.Vitesse_Consigne);
  printf("Vitesse Reelle %f\n", controle_v7_1_U.Vitesse_Reelle);
  printf("Light %f\n", controle_v7_1_U.Light);

  /* Step the model for base rate */
  controle_v7_1_step();

  

  /* Get model outputs here */
  // Log all outputs here
  printf("Vitesse consigne (Output) %d\n", (int) controle_v7_1_Y.Vitesse_Envoyee);

  train->speed = (int)  controle_v7_1_Y.Vitesse_Envoyee;

  /* Indicate task complete */
  OverrunFlag = false;


  printf("\nonestep end\n");

  gettimeofday(&lastCall, NULL);
  printf("\nDuration of onestep: %0.8f\n", time_diff(&currentCall, &lastCall));

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

void onestep_2(Train *train)
{
  static boolean_T OverrunFlag = false;
  // time between last call

  printf("\nonestep_2 begin\n");
  printf("Current train id: %d\n", train->id);

  

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(controle_v7_2_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */
  //if(train->order > )
  //train->pos-selectTrainByOrder()
  controle_v7_2_U.Distance = 200;
  controle_v7_2_U.Vitesse_Consigne = 20;
  controle_v7_2_U.Vitesse_Reelle = train->speedMeasured;
  controle_v7_2_U.Light = 0;

  // Log all inputs
  printf("Distance %f\n", controle_v7_2_U.Distance);
  printf("Vitesse Consigne (Input) %f\n", controle_v7_2_U.Vitesse_Consigne);
  printf("Vitesse Reelle %f\n", controle_v7_2_U.Vitesse_Reelle);
  printf("Light %f\n", controle_v7_2_U.Light);

  /* Step the model for base rate */
  controle_v7_2_step();

  

  /* Get model outputs here */
  // Log all outputs here
  printf("Vitesse consigne (Output) %d\n", (int) controle_v7_2_Y.Vitesse_Envoyee);

  train->speed = (int)  controle_v7_2_Y.Vitesse_Envoyee;

  /* Indicate task complete */
  OverrunFlag = false;


  printf("\nonestep end\n");

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

void * orderTrain2(Train * trains){
    while(trains->id!=0){   
        trains->order=trains->id;
        trains=trains->nextTrain;
    }
    return NULL;
}


void* connection_handler(void *socket_desc){
    int sock = *(int*)socket_desc;
    char message[MAXLEN];
    T_list list = NULL;
    int code;
    int id;
    int pos;
    int speed;
    int read;

    do {
            read = recv(sock , message , MAXLEN , 0);
            if (read < 0) {
                    perror("Reading stream message");
            } else if (read == 0) {
                // TODO remove train
                perror("Ending connection\n");
                break;
            } else {
                list = NULL;
                list = splitMessages(list, message);
                showList(list);
                while (list != NULL)
                {
                    list = parseMessage(list, &code, &id, &pos, &speed);
                   

                    printf("Responding new message from %i with code %i\n", id, code);
                    switch(code){

                        case 1: // ack connection
                            printf("Received initialization from train %i\n", id);
                            if (!selectTrain(id, trainsList))
                            {
                                trainsList = addTrain(id, pos, speed, sock, trainsList);
                                // pthread_mutex_lock(&mutex);
                                // printf("in mutex\n");
                                orderTrain2(trainsList);
                                // pthread_mutex_unlock(&mutex);
                                //printf("after order train");
                            }
                            sendData(sock, 2, id, -1, -1);
                            break;
                        case 3: // receive position/speed
                            // store data
                            printf("Received speed/position from train %i\n", id);
                            Train * train = selectTrain(id, trainsList);
                            if (!train)
                            {
                                trainsList =  addTrain(id, pos, speed, sock, trainsList); 
                                // pthread_mutex_lock(&mutex);
                                orderTrain2(trainsList);
                                // pthread_mutex_unlock(&mutex);
                                train = trainsList;
                            }
                            else
                            {
                                storeData(id,pos,speed,trainsList);
                            }
                            //onestep(train);
                            //sendData(sock, 4, id, pos, train->speed); //send ack 
                            break;
                        case 5: // send command
                            printf("Speed request ack from train %i\n", id);
                            break;
                        default:
                            break;
                    }
                    //showTrains(trainsList);
                    // list = removeFirstNode(list);
                    // printf("SPEED train %d: %f \n", id, calcSpeed(selectTrain(id, trainsList), trainsList));
                    //sendData(sock, 6, id, -1, 40);

                    //showList(list);
                    printf("Message done!\n");
                };
            }
    } while(1);
    return NULL;
}

static Train * newTrain(int id, int pos, int speed, int sock) {
	Train * train; 
	CHECK_IF(train = malloc(sizeof(Train)), NULL, "malloc allocateNode"); 
	train->id = id; 
	train->pos = pos; 
	train->speed = speed; 
    train->order = 0;
    train->sock = sock;
	train->nextTrain = NULL;
	return train;
}


Train * addTrain (int id, int pos, int speed, int sock, Train *trains) { 
	Train * train; 
	train = newTrain(id, pos, speed, sock); 
	train->nextTrain = trains;
	return train;
}

void showTrains(Train *trains) {
	while(trains!= NULL) {
		printf(" => Train numéro : %d \n",trains->id);
        printf("  --------------------\n");
		printf("\tpos : %d \n",trains->pos);
		printf("\tspeed : %d \n",trains->speed);
		printf("\torder : %d \n\n",trains->order);
		trains= trains->nextTrain; 
	}
}

Train * selectTrain (int id_train, Train *trains) {
    while(trains!=NULL && id_train!=trains->id){
        trains=trains->nextTrain;
    }
    return trains;
} 

Train * selectTrainByOrder (int orderTrain, Train *trains) {
    while(trains!=NULL && orderTrain!=trains->order){
        trains=trains->nextTrain;
    }
    return trains;
} 

void * storeData(int id_train, int pos, int speed, Train * trains){
    trains=selectTrain(id_train, trains);
    trains->pos=pos;
    trains->speedMeasured=speed;
    return NULL;
}

void * orderTrain(Train * listTrains){
    // ici on cherche le plus grand écart entre les trains et on définit order=1 pour le train qui est à la tete
    int distmax=0;
    Train * trains=listTrains;
    Train * firstTrain=listTrains;
    int numberOfTrains=0;
    while(trains->id!=0){
        trains->order=-1; //réinitialise les ordres 
        Train *nextTrains=trains->nextTrain;
        while(nextTrains->id!=0){ //on mets a jour la distance max et on attribue momentanément le premier train
            int dist = trains->pos-nextTrains->pos;
            if(dist>distmax && dist<roundf(DISTTOUR/2)) {distmax=dist; firstTrain=trains;}; 
            if(dist>distmax && dist>roundf(DISTTOUR/2)) {distmax=dist; firstTrain=nextTrains;};
            if(-dist>distmax && -dist<roundf(DISTTOUR/2)) {distmax=-dist; firstTrain=nextTrains;};
            if(-dist>distmax && -dist>roundf(DISTTOUR/2)) {distmax=-dist; firstTrain=trains;};
            nextTrains=nextTrains->nextTrain;
        }
        numberOfTrains += 1;
        trains = trains->nextTrain;
    }

    firstTrain->order=1;

    if(numberOfTrains>1){
        Train *nextTrain=firstTrain;
        for(int i=2;i<=numberOfTrains;i++){
            nextTrain=selectTrain(calcMinDistance(nextTrain->id, listTrains), listTrains);
            nextTrain->order=i;
        }
    }
    return NULL;
}

int calcMinDistance(int id_train, Train * trainsList){ //fonction utilisée uniquement par orderTrain pour classer les trains en fonction de la plus petite distance qui les sépare 
    Train *nextTrain = selectTrain(id_train, trainsList);
    // chercher le train suivant (position sup, à un tour pres)
    int distmin, id;
    while(trainsList->id!=0){
        if(trainsList->id!=id_train && trainsList->order!=0){ // on parcours les trains qui ne sont pas encore ordonnés et qui ne soient pas le pc au sol (pos=-1)
            int dist=nextTrain->pos - trainsList->pos;
            if(dist>0 && dist<distmin){distmin=dist; id=trainsList->id;};
            if(dist<0 && -dist<distmin) {distmin=-dist; id=trainsList->id;};
            if(dist<0 && DISTTOUR+dist<distmin) {distmin=DISTTOUR+dist; id=trainsList->id;};
            if(dist==0){id=0;break;}
        printf("calcmin %d, %d \n",trainsList->pos, dist);
        }
        trainsList=trainsList->nextTrain;
    }
        printf("out%i\n ", id);
    return id;
}

int calcDistance(Train *train1, Train *train2){ //attention pour utiliser cette fonction il faut que train1->order > train2->order
    int dist=train1->pos-train2->pos;
    if(dist<0) dist+=DISTTOUR;
    return dist;
}

float calcSpeed(Train *train, Train * trainsList){

    if (train->id == 0) return -1;
    else if (train->order == 1)
    {
        return MAX_SPEED;
    }
    else {   //ici il faut distinguer le cas ou le train est trop proche du suivant ou s'il peut aller a vmax
        if(trainsList->nextTrain->id == 0) return MAX_SPEED; // cas ou le train est seul

        Train *precedTrain=selectTrainByOrder(train->order - 1, trainsList);
        int dist = calcDistance(precedTrain, train);

        //calc de la distance reglementaire en fonction de la vitesse du train de devant 
        int dist_ok=precedTrain->speed * 2; //distance parcourue en deux sec

        int e = dist_ok - dist;

        if (dist < EMERGENCY_DIST){
            printf("STOOOOP\n");
            return 0.0;
        }
        else if (dist < dist_ok){
            printf("ral \n");
            return 0.9 * (train->speed);
        }
        else{
            printf("placement\n");
            float K = fabs(GAIN / e * (train->speed));
            if (K > MAX_SPEED)
            {
                return MAX_SPEED;
            }
            else
            {
                return K;
            }
        }
    }
    return 0.0;
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    int port;
    int sock;
    struct sockaddr_in server, client;



    struct itimerval timer;
    timer.it_interval.tv_sec=0;
	timer.it_interval.tv_usec=34000; 
	timer.it_value=timer.it_interval;

    // Add clock 
	if (signal(SIGALRM, periodSending) == SIG_ERR)
 		printf("\nCannot catch signal SIGALARM\n");

    setitimer(ITIMER_REAL, &timer,NULL);

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Opening stream socket");
        exit(1);
    }

    /* Custom port */
    if (argc == 2) {
		port = atoi(argv[1]);
	} else {
		port = PORT_NUMBER;
	}
    printf("Port: %d\n", port);

    /* Name socket */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)))
    {
        perror("Binding stream socket");
        exit(1);
    }

    /* Accepting incoming connections */
    listen(sock, MAX_REQUEST);
    puts("Listening");
    int c = sizeof(struct sockaddr_in);
    int new_socket;
    trainsList=addTrain(0,-1,-1,-1,NULL);


    gettimeofday(&lastCall, NULL);

    while ((new_socket = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)))
    {
        int *new_sock = malloc(1);
        *new_sock = new_socket;

        /* Start a new thread to handle the connection */
        if(pthread_create(&thread, NULL, connection_handler, (void*) new_sock)<0)
        {
            perror("Could not create thread");
            return 1;
        }
        pthread_detach(thread);
        puts("New connection assigned to handler");
    }
    controle_v7_1_terminate();
    controle_v7_2_terminate();
}