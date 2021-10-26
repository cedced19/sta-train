#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "RBC.h"

#define MAXLEN 500

Train *trainsList=NULL;

void* connection_handler(void *socket_desc){
    int sock = *(int*)socket_desc;
    char message[MAXLEN];
    int read_size;
    T_list list = NULL;
    int code;
    int id;
    int pos;
    int speed;


    do {
            int read = read_size = recv(sock , message , MAXLEN , 0);
            if (read < 0) {
                    perror("Reading stream message");
            } else if (read == 0) {
                // TODO remove train
                perror("Ending connection\n");
                break;
            } else {
                list = getOneMessage(list,message);
                while(list!=NULL) {
                    if(parseMessage(list->data, &code, &id, &pos, &speed) == 0) {
                        if (list!=NULL) {
                            list = removeFirstNode(list);
                        }
                        break;
                    }
                    printf("Responding new message from %i with code %i\n", id, code);
                    switch(code){
                        case 1: // receive position/speed
                            // store data
                            printf("Received initialization from train %i\n", id);
                            if (!selectTrain(id, trainsList)) {
                                trainsList = addTrain(id,pos,speed, trainsList);  
                                orderTrain(trainsList);
                            } else {
                                storeData(id,pos,speed,trainsList);
                            }
                            sendData(sock, 2, id, pos, speed); //send ack 
                            break;
                        case 3 : // send command
                            printf("speed request ack from train %i\n", id);
                            break;
                        default:
                            break;
                    }
                    showTrains(trainsList);
                    list = removeFirstNode(list);
                    sendData(sock, 4, id, -1, calcSpeed(selectTrain(id, trainsList)));

                    //showList(list);
                    printf("Message done!\n");
                
                };
            }
    } while(1);
    return NULL;
}

static Train * newTrain(int id, int pos, int speed) {
	Train * train; 
	CHECK_IF(train = malloc(sizeof(Train)), NULL, "malloc allocateNode"); 
	train->id = id; 
	train->pos = pos; 
	train->speed = speed; 
    train->order = 0;
	train->nextTrain = NULL;
	return train;
}


Train * addTrain (int id, int pos, int speed, Train *trains) {
	Train * train; 
	train = newTrain(id, pos, speed); 
	train->nextTrain = trains; 
	return train;
}

void showTrains(Train *trains) {
	while(trains!= NULL) {
		printf(" => Train numéro : %d \n",trains->id);
        printf("  --------------------\n");
		printf("\tpos : %d \n",trains->pos);
		printf("\tspeed : %d \n\n",trains->speed);
		trains= trains->nextTrain; 
	}
}

Train * selectTrain (int id_train, Train *trains) {
    while(trains!=NULL && id_train!=trains->id){
        trains=trains->nextTrain;
    }
    return trains;
} 

void * storeData(int id_train, int pos, int speed, Train * trains){
    trains=selectTrain(id_train, trains);
    trains->pos=pos;
    trains->speed=speed;
    return NULL;
}

void * orderTrain(Train * listTrains){
    // ici on cherche le plus grand écart entre les trains et on définit order=1 pour le train qui est à la tete
    int distmax=0;
    Train * trains=listTrains;
    Train * firstTrain=NULL;
    int numberOfTrains=0;
    while(trains!=NULL){
        trains->order=0; //réinitialise le premier train
        Train *nextTrains=trains->nextTrain;
        while(nextTrains!=NULL){ //on mets a jour la distance max et on attribue momentanément le premier train
            int dist = trains->pos-nextTrains->pos>distmax;
            if(dist>distmax && dist<roundf(DISTTOUR/2)) {distmax=dist; firstTrain=trains;}; 
            if(dist>distmax && dist>roundf(DISTTOUR/2)) {distmax=dist; firstTrain=nextTrains;};
            if(-dist>distmax && -dist<roundf(DISTTOUR/2)) {distmax=-dist; firstTrain=nextTrains;};
            if(-dist>distmax && -dist>roundf(DISTTOUR/2)) {distmax=-dist; firstTrain=trains;};
            nextTrains=nextTrains->nextTrain;
        }
        numberOfTrains += 1;
        trains = trains->nextTrain;
    }
    firstTrain->order=0;
    for(int i=1;i<numberOfTrains;i++){
        Train *nextTrain;
        nextTrain=selectTrain(calcMinDistance(i, trains), listTrains);
        nextTrain->order=i;
    }
    return NULL;
}

int calcMinDistance(int id_train, Train * trainsList){ //fonction utilisée uniquement par orderTrain pour classer les trains en fonction de la plus petite distance qui les sépare 
    Train *nextTrain = selectTrain(id_train, trainsList);
    // chercher le train suivant (position sup, à un tour pres)
    int distmin, id;
    while(trainsList!=NULL){
        if(trainsList->id!=id_train && trainsList->pos!=-1 && !(trainsList->order)){ // on parcours les trains qui ne sont pas encore ordonnés et qui ne soient pas le pc au sol (pos=-1)
            int dist=nextTrain->pos - trainsList->pos;
            if(dist>0 && dist<distmin){distmin=dist; id=nextTrain->id;};
            if(dist<0 && DISTTOUR+dist<distmin) {distmin=DISTTOUR+dist; id=nextTrain->id;};
        }
        trainsList=trainsList->nextTrain;
    }
    return id;
}

int calcDistance(Train *train1, Train *train2){ //attention pour utiliser cette fonction il faut que train1->order > train2->order
    int dist=train1->pos-train2->pos;
    if(dist<0) dist+=DISTTOUR;
    return dist;
}

float calcSpeed(Train *train){

    if (train->order == 0)
    {
        return 30.0;
    }
    else {   //ici il faut distinguer le cas ou le train est trop proche du suivant ou s'il peut aller a vmax
    //     int d = 0;
    //     if(trains.nb_trains >= 2){
    //         d  = distance(t1, t2);
    //     }
    //     else{
    //         d = 1745;
    //     }

    //     int e = DIST_OPT - d;

    //     if (d < DIST_STOP)
    //     {
    //         printf("Emergency stop !");
    //         return 0;
    //     }
    //     else if (d < DIST_SLOW_DOWN)
    //     {
    //         printf("Slow mode");
    //         return 0.9 * (t1 -> speed);
    //     }
    //     else
    //     {
    //         printf("Regulation mode");
    //         float K = fabs(P / e * (t1 -> speed));
    //         if (K > 25)
    //         {
    //             return 25;
    //         }
    //         else
    //         {
    //             return K;
    //         }
    //     }
    // }
    //     return ;
    }
    return 0.0;
}

int main()
{
    pthread_t thread;

    int sock;
    struct sockaddr_in server, client;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Opening stream socket");
        exit(1);
    }

    /* Name socket */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NUMBER);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server))) {
        perror("Binding stream socket");
        exit(1);
    }

    /* Accepting incoming connections */
    listen(sock, MAX_REQUEST);
    puts("Listening");
    int c = sizeof(struct sockaddr_in);
    int new_socket;
    trainsList=addTrain(0,-1,-1,NULL);

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

}