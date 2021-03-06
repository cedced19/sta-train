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


struct timeval lastCall_1;
struct timeval currentCall_1;

struct timeval lastCall_2;
struct timeval currentCall_2;

void onestep_1(Train *train);
void onestep_2(Train *train);

Train *trainsList=NULL;

float time_diff(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}


int socket_display=0;
int isconnected_display=0;
void sendDisplay(int code, int id, int pos, int speed, Train* train) {
    struct timeval now;
    gettimeofday(&now, NULL);
    if(isconnected_display){
        
        if (time_diff(&(train->lastDisplay), &now)>0.02) {
            sendData(socket_display, code, id, pos, speed);
            printf("Send display after %0.5f\n", time_diff(&(train->lastDisplay), &now));
            gettimeofday(&(train->lastDisplay), NULL);
        }
        
    }
    
}

//////////////////////////////////////////
/// Signal handler
/// SIGALARM
///////////////////////////////////////////
void periodSending(int signo) {
    Train* train = trainsList;
    //showTrains(train);
	while(train != NULL) {
        if (train->pos != -1 && train->id != 0 && train->connected == 1) {
            if (train->initialized == 0) {
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
            
            if(sendData(train->sock, 6, train->id, train->pos, train->speed) == 0) {
                train->connected = 0;
                train->initialized = 0;
                if (train->id == 1) {
                    controle_v7_1_terminate();
                }
                if (train->id == 2) {
                    controle_v7_2_terminate();
                }
            }
            sendDisplay(6, train->id, train->pos, train->speed, train);
        }
        train = train->nextTrain;
    }
} 

void writeCSV (float onestep_duration, float period, Train* train) {
    FILE *f;
    
    char filename[12];
    sprintf(filename,"train_%d.csv", train->id);

    if (train->initText) {
        f = fopen(filename, "wb");
        train->initText = 0;
        fprintf(f, "onestep_duration,period,distance,vitesse_consigne,vitesse_reelle,vitesse_envoyee\n");
    } else {
        f = fopen(filename, "a+");
        fprintf(f, "%0.4f,", onestep_duration);
        fprintf(f, "%0.4f,", period);
        fprintf(f, "%0.1f,", controle_v7_1_U.Distance);
        fprintf(f, "%0.1f,", controle_v7_1_U.Vitesse_Consigne);
        fprintf(f, "%0.1f,", controle_v7_1_U.Vitesse_Reelle);
        fprintf(f, "%0.1f\n", controle_v7_1_Y.Vitesse_Envoyee);
    }
    fclose(f);
}


void onestep_1(Train *train)
{
  static boolean_T OverrunFlag = false;
  gettimeofday(&currentCall_1, NULL);
  // time between last call
  float period = time_diff(&lastCall_1, &currentCall_1);
  //printf("\nTime between last call of onestep: %0.8f s\n", time_diff(&lastCall, &currentCall));

  //printf("\nonestep_1 begin\n");
  //printf("Current train id: %d\n", train->id);

  

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(controle_v7_1_M, "Overrun");
    return;
  }

  OverrunFlag = true;


  int defaultConsigne = 20;
  if (train->speedConsigne !=0) {
      defaultConsigne = train->speedConsigne;
  }

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */
  //if(train->order > )
  //train->pos-selectTrainByOrder()

  int defaultDistance = 4000;
  int otherTrainsId[2] = {2,3};
  for(int i = 0; i < 2; ++i) {
      Train* trainref = selectTrain(otherTrainsId[i],trainsList);
      if (trainref != NULL) {
          defaultDistance=trainref->pos-train->pos;
          if(defaultDistance<0) defaultDistance+=DISTTOUR;
          break;
      } 
  }
    /*if(train->order>1){
        Train *trainPrec=selectTrainByOrder(train->order - 1, trainsList);
        defaultDistance = calcDistance(trainPrec, train);
    }*/

  controle_v7_1_U.Distance = defaultDistance;
  controle_v7_1_U.Vitesse_Consigne = defaultConsigne;
  controle_v7_1_U.Vitesse_Reelle = train->speedMeasured;
  controle_v7_1_U.Light = 0;
  
  printf("Distance 1->2 %f\n", controle_v7_1_U.Distance);
  // Log all inputs
  //printf("Distance %f\n", controle_v7_1_U.Distance);
  //printf("Vitesse Consigne (Input) %f\n", controle_v7_1_U.Vitesse_Consigne);
  ///printf("Vitesse Reelle %f\n", controle_v7_1_U.Vitesse_Reelle);
  //printf("Light %f\n", controle_v7_1_U.Light);

  /* Step the model for base rate */
  controle_v7_1_step();

  

  /* Get model outputs here */
  // Log all outputs here
  //printf("Vitesse consigne (Output) %d\n", (int) controle_v7_1_Y.Vitesse_Envoyee);

  train->speed = (int)  controle_v7_1_Y.Vitesse_Envoyee;

  /* Indicate task complete */
  OverrunFlag = false;


  //printf("\nonestep end\n");

  gettimeofday(&lastCall_1, NULL);
  //printf("\nDuration of onestep: %0.8f\n", );

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
  writeCSV(time_diff(&currentCall_1, &lastCall_1), period, train);
}

void onestep_2(Train *train)
    {
    static boolean_T OverrunFlag = false;
    // time between last call

    //printf("\nonestep_2 begin\n");
    //printf("Current train id: %d\n", train->id);
    gettimeofday(&currentCall_2, NULL);
    // time between last call
    float period = time_diff(&lastCall_2, &currentCall_2);
    
    
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
    int defaultConsigne = 35;
    if (train->speedConsigne!=0) {
        defaultConsigne = train->speedConsigne;
    }

    int defaultDistance = 4000;
    /*if(train->order>1){
        Train *trainPrec=selectTrainByOrder(train->order - 1, trainsList);
        defaultDistance = calcDistance(trainPrec, train);
    }*/

    int otherTrainsId[2] = {1,3};
    for(int i = 0; i < 2; ++i) {
        Train* trainref = selectTrain(otherTrainsId[i],trainsList);
        if (trainref != NULL) {
            defaultDistance=trainref->pos-train->pos;
            if(defaultDistance<0) defaultDistance+=DISTTOUR;
            break;
        } 
    }

    controle_v7_2_U.Distance = defaultDistance;
    controle_v7_2_U.Vitesse_Consigne = defaultConsigne;
    controle_v7_2_U.Vitesse_Reelle = train->speedMeasured;
    controle_v7_2_U.Light = 0;

    // Log all inputs
    printf("Distance 2->1 %f\n", controle_v7_2_U.Distance);
    //printf("Vitesse Consigne (Input) %f\n", controle_v7_2_U.Vitesse_Consigne);
    //printf("Vitesse Reelle %f\n", controle_v7_2_U.Vitesse_Reelle);
    //printf("Light %f\n", controle_v7_2_U.Light);

    /* Step the model for base rate */
    controle_v7_2_step();

    

    /* Get model outputs here */
    // Log all outputs here
    //printf("Vitesse consigne (Output) %d\n", (int) controle_v7_2_Y.Vitesse_Envoyee);

    train->speed = (int)  controle_v7_2_Y.Vitesse_Envoyee;

    /* Indicate task complete */
    OverrunFlag = false;


    gettimeofday(&lastCall_2, NULL);
    //printf("\nDuration of onestep: %0.8f\n", );

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
    writeCSV(time_diff(&currentCall_2, &lastCall_2), period, train);
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
    Train * selected = NULL;

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
                //showList(list);
                while (list != NULL)
                {
                    list = parseMessage(list, &code, &id, &pos, &speed);
                    if (id==-1) {
                        printf("GUI\n");
                        isconnected_display=1;
                        //printf("%d\n", isconnected_display);
                        socket_display=sock;
                        // Here pos represent which train we whant to affect the speed consigne
                        if (pos != -1 && code == 10) {
                            selected = selectTrain(pos, trainsList);
                            if (selected != NULL) {
                                selected->speedConsigne = speed;
                                printf("Speed consigne from train %d changed to %d.\n", pos, speed);
                            }
                            
                        }
                        continue; // the display do not send any messages
                    }
                   

                    //printf("Responding new message from %i with code %i\n", id, code);
                    switch(code){

                        case 1: // ack connection
                            //printf("Received initialization from train %i\n", id);
                            if (!selectTrain(id, trainsList))
                            {
                                trainsList = addTrain(id, pos, speed, sock, trainsList);
                                // pthread_mutex_lock(&mutex);
                                // printf("in mutex\n");
                                orderTrain2(trainsList);
                                // pthread_mutex_unlock(&mutex);
                                //printf("after order train");
                            }
                            trainsList->connected = 1;
                            sendData(sock, 2, id, -1, -1);
                            break;
                        case 3: // receive position/speed
                            // store data
                            // printf("Received speed/position from train %i\n", id);
                            Train * train = selectTrain(id, trainsList);
                            if (!train)
                            {
                                trainsList = addTrain(id, pos, speed, sock, trainsList); 
                                // pthread_mutex_lock(&mutex);
                                orderTrain2(trainsList);
                                // pthread_mutex_unlock(&mutex);
                                train = trainsList;
                            }
                            else
                            {
                                storeData(id,pos,speed,trainsList);
                                if (train->connected == 0) {
                                    train->sock = sock;
                                }
                            }
                            train->connected = 1;
                            
                            //onestep(train);
                            //sendData(sock, 4, id, pos, train->speed); //send ack 
                            sendDisplay(4, id, pos, speed, train);
                            break;
                        case 5: // send command
                            // printf("Speed request ack from train %i\n", id);
                            break;
                        default:
                            break;
                    }
                    //showTrains(trainsList);
                    // list = removeFirstNode(list);
                    // printf("SPEED train %d: %f \n", id, calcSpeed(selectTrain(id, trainsList), trainsList));
                    //sendData(sock, 6, id, -1, 40);

                    //showList(list);
                    //printf("Message done!\n");
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
    train->connected = 0;
    train->initialized = 0;
    train->initText = 1;
    train->sock = sock;
	train->nextTrain = NULL;
    train->speedConsigne = 0;
    gettimeofday(&train->lastDisplay, NULL);
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
		printf(" => Train num??ro : %d \n",trains->id);
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
    while(trains!=NULL){
        if(trains->order == orderTrain)
            return trains;
        trains=trains->nextTrain;
    }
    return NULL;
} 

void * storeData(int id_train, int pos, int speed, Train * trains){
    trains=selectTrain(id_train, trains);
    trains->pos=pos;
    trains->speedMeasured=speed;
    return NULL;
}

void * orderTrain(Train * listTrains){
    // ici on cherche le plus grand ??cart entre les trains et on d??finit order=1 pour le train qui est ?? la tete
    int distmax=0;
    Train * trains=listTrains;
    Train * firstTrain=listTrains;
    int numberOfTrains=0;
    while(trains->id!=0){
        trains->order=-1; //r??initialise les ordres 
        Train *nextTrains=trains->nextTrain;
        while(nextTrains->id!=0){ //on mets a jour la distance max et on attribue momentan??ment le premier train
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

int calcMinDistance(int id_train, Train * trainsList){ //fonction utilis??e uniquement par orderTrain pour classer les trains en fonction de la plus petite distance qui les s??pare 
    Train *nextTrain = selectTrain(id_train, trainsList);
    // chercher le train suivant (position sup, ?? un tour pres)
    int distmin, id;
    while(trainsList->id!=0){
        if(trainsList->id!=id_train && trainsList->order!=0){ // on parcours les trains qui ne sont pas encore ordonn??s et qui ne soient pas le pc au sol (pos=-1)
            int dist=nextTrain->pos - trainsList->pos;
            if(dist>0 && dist<distmin){distmin=dist; id=trainsList->id;};
            if(dist<0 && -dist<distmin) {distmin=-dist; id=trainsList->id;};
            if(dist<0 && DISTTOUR+dist<distmin) {distmin=DISTTOUR+dist; id=trainsList->id;};
            if(dist==0){id=0;break;}
        printf("calcmin %d, %d \n",trainsList->pos, distmin);
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


    gettimeofday(&lastCall_1, NULL);
    gettimeofday(&lastCall_2, NULL);

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