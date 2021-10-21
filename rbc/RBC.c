#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
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
                            if (selectTrain(id, trainsList) == NULL) {
                                trainsList = addTrain(id,pos,speed, trainsList);    
                            } else {
                                storeData(id,pos,speed,trainsList);
                            }
                            sendData(sock, 2, id, pos, speed); //send ack 
                            //showTrains(trainsList);
                            break;
                        case 3 : // send command
                            printf("Received position from train %i\n", id);
                            storeData(id,pos,speed,trainsList);
                            //showTrains(trains);
                            sendData(sock, 6, id, pos, 45); //send ack 
                            break;
                        default:
                            break;
                    }
                    showTrains(trainsList);
                    
                    list = removeFirstNode(list);
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

int calcDistance(int id_train){
    Train *train1 = selectTrain(id_train, trainsList);
    // chercher le train suivant (position sup, à un tour pres)
    int distmin;
    while(trainsList!=NULL){
        if(trainsList->id!=id_train){
            if(abs(train1->pos - trainsList->pos)<distmin) distmin=abs(train1->pos - trainsList->pos);
            if(abs(train1->pos - trainsList->pos + DISTTOUR)<distmin) distmin=abs(train1->pos - trainsList->pos + DISTTOUR);
            if(abs(train1->pos - trainsList->pos - DISTTOUR)<distmin) distmin=abs(train1->pos - trainsList->pos - DISTTOUR);
        }
    }
    return distmin;
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